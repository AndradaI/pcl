//
//  Tree.cpp
//  Phylogenetic Class Library
//
//  Created by mbrazeau on 28/06/2017.
//  Copyright © 2017 Brazeau Lab. All rights reserved.
//

#include "Tree.hpp"
#include "Topology.hpp"

bool Tree::isrooted(void)
{
    return _isrooted;
}

unsigned long Tree::size(void)
{
    return _tips.size() + _internals.size();
}

unsigned long Tree::capacity(void)
{
    return _nodes.size()-1;
}

void Tree::restore(Topology &topol)
{
    int i = 0;
    unsigned long index = 0;
    unsigned long anci = 0;
    unsigned long max = topol.size();
    
    for (i = 0; i < max; ++i)
    {
        index = topol.index(i);
        anci = topol.edge(i);
        
        std::cout << index << std::endl;
        std::cout << anci << ',' << std::endl;
        _nodes[anci]->addDescendant(*_nodes[index]);
        _nodes[index]->_tip = topol.tipnumber(i);
    }
    
    _start = _reserved_root;
    
    traverse();
    
    //std::cout << "Not implemented\n";
}

void Tree::reset(void)
{
    
    std::vector<Node*>::iterator it;
    
    
    for (it = _nodes.begin(); it != _nodes.end(); ++it) {
        Node* n = *it;
        n->disconnectAll();
    }
    
    _postorder.clear();
    _tips.clear();
    _internals.clear();
    _start = NULL;
    _natural_score = 0;
    _real_score = 0.0;
    _nextFreeTip = _nodes.begin();
    _nextFreeInternal = _nodes.begin() + _num_taxa;
}

void Tree::incrScore(unsigned long s)
{
    _natural_score += s;
}

void Tree::incrScore(double s)
{
    _real_score += s;
}

void Tree::setScore(unsigned long s)
{
    _natural_score = 0;
}

void Tree::setScore(double s)
{
    _real_score = s;
}

void Tree::clearScore(void)
{
    _natural_score = 0;
    _real_score = 0.0;
}

unsigned long Tree::natScore(void)
{
    return _natural_score;
}

double Tree::realScore(void)
{
    return _real_score;
}

Node* Tree::newTip(int id_number)
{
    if (_nextFreeTip != _nodes.begin() + _num_taxa)
    {
        Node *ret = *_nextFreeTip;
        ret->_tip = id_number;
        ++_nextFreeTip;
        return ret;
    }
    
    return NULL;
}

Node* Tree::newVertex(void)
{
    if (_nextFreeInternal != _nodes.end()-1)
    {
        Node *ret = *_nextFreeInternal;
        ++_nextFreeInternal;
        return ret;
    }
    
    return NULL;
}

Node* Tree::rootNode(void)
{
    return _reserved_root;
}

void Tree::setStart(Node &n)
{
    _start = &n;
}

void Tree::root(int index)
{

    bool popstart = false;
    
    if (isrooted()) {
        // Pop out the internal starting node
        popstart = true;
    }
    
    markDownpass(index);
    
    // TODO: Pop out root
    
    unroot();
    
    int i = 0;
    int max = _reserved_root->memIndex();
    
    for (i = 0; i < max; ++i) {
        if (_nodes[i]->_in_path == true) {
            _nodes[i]->rotate();
        }
    }
   
    for (i = 0; i < max; ++i) {
        _nodes[i]->_in_path = false;
    }
    
    root();
    
    traverse();
}

/*!
 @brief Quickly roots the tree at the current start position.
 @discussion A fast rooting procedure that simply inserts the root at the edge
 next to the current entry point on the tree. This obviates any need to rotate
 internal nodes and thus proceeds faster. This function is useful when a quick
 rooting needs to be applied to an unrooted tree for calculations, Newick 
 writing etc.
 */
void Tree::root(void)
{
    if (isrooted()) {
        return;
    }
    Node* p = NULL;
    Node* q = NULL;
    
    p = _start;
    q = _start->parent();
    
    _reserved_root->addDescendant(*p);
    _reserved_root->addDescendant(*q);
    
    _isrooted = true;
}

/*!
 @brief Unroots the tree.
 @discussion Removes the root of the tree and points the tree entry to the the
 internal node joining the tip with the numerically lowest index. That tip is 
 now the ancestor of the start. If the root already neighbours this tip, then
 no further operations are made. Otherwise, the tree is redirected (i.e. re-
 'rooted') to place the lowest-numbered tip next to the start.
 */
void Tree::unroot(void)
{
    int index = 0;
    
    index = _tips[0]->memIndex(); // Get the lowest tip in the tree
    
    // TODO: Need to accommodate different root configurations and implement the description accurately.
    _reserved_root->left()->parent(*_start->right());
    _reserved_root->right()->parent(*_start->left());
    _reserved_root->_left = NULL;
    _reserved_root->_right = NULL;
    
    _isrooted = false;
}

void Tree::traverse(void)
{
    _postorder.clear();
    _tips.clear();
    _internals.clear();
    _start->traverse(_postorder, _tips, _internals);
}

void Tree::markDownpass(int index)
{
    bool found = false;
    
    // TODO: Make safe for
    _start->markTraverse(index, &found);
    
    // TODO: Unmark all
}

