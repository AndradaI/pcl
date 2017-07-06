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

unsigned long Tree::numVertices(void)
{
    return _internals.size();
}

int Tree::numTaxa(void)
{
    return _num_taxa;
}

int Tree::numTips(void)
{
    return static_cast<int>(_tips.size());
}

void Tree::restore(Topology &topol)
{
    reset();
    int i = 0;
    unsigned long index = 0;
    unsigned long anci = 0;
    unsigned long max = topol.size();
    
    for (i = 0; i < max; ++i)
    {
        index = topol.index(i);
        anci = topol.edge(i);
        
        _nodes[anci]->addDescendant(*_nodes[index]);
        _nodes[index]->_tip = topol.tipnumber(i);
    }
    
    _start = _reserved_root;
    
    traverse();
}

void Tree::reset(void)
{
    
    std::vector<Node*>::iterator it;
    
    _free_tips.clear();
    _free_vertices.clear();
    
    for (it = _nodes.begin(); it != _nodes.end(); ++it) {
        Node* n = *it;
        n->disconnectAll();
        if (n->tipNumber() != 0)
        {
            _free_tips.push_back(n);
        }
        else {
            _free_vertices.push_back(n);
        }
    }
    
#ifdef DEBUG
    int i = 0;
    std::vector<Node*>::iterator dbit = _free_tips.begin();
    unsigned long int listmax = _free_tips.size()-1;
    for (i = 0; i < listmax; ++i) {
        assert((*dbit)->tipNumber() == i + 1);
        ++dbit;
    }
#endif
    
    connectDummy();
    
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

Node* Tree::node(int index)
{
    return _nodes[index];
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

    //traverse();
    
    bool popstart = false;
    
    if (isrooted()) {
        // Pop out the internal starting node
        popstart = true;
    }
    
    Node* target = NULL;
    target = markDownpass(index);
    
    // TODO: Pop out root
    
    Node* bleft = _start->_left;
    Node* bright = _start->_right;
    bleft->parent(*bright);
    bright->parent(*bleft);
    
    //int max = _reserved_root->memIndex();
    std::vector<Node*>::iterator i;
    
    for (i = _internals.begin(); i != _internals.end(); ++i) {
        
        if (*i == _start) {
            break;
        }

        if ((*i)->_in_path == true) {
            (*i)->rotate();
        }
    }
   
    for (i = _postorder.begin(); i != _postorder.end(); ++i) {
        (*i)->_in_path = false;
    }
    
    //_start = _reserved_root;
    _start->_descs.clear();
    _start->_descs.push_back(target);
    _start->_descs.push_back(target->parent());
    _start->_left = _start->_descs.front();
    _start->_right = _start->_descs.back();
    //    _start->_left = _nodes[index];
    //_start->_right = _nodes[index]->parent();
    _start->_left->parent(*_start);
    _start->_right->parent(*_start);
    
    //root();
    traverse();
}

void Tree::root(Node &n)
{
    int index = n.memIndex();
    root(index);
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
    
    _start = _reserved_root;
    
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
    
    std::cout << "\n";
}

Node* Tree::postorder(int index)
{
    return _postorder[index];
}

Node* Tree::postorderIntern(int index)
{
    return _internals[index];
}

Node* Tree::preorder(int index)
{
    return _postorder[_postorder.size() - index - 1];
}

Node* Tree::preorderIntern(int index)
{
    return _internals.back()-index;
}

Node* Tree::tip(int index)
{
    return _tips[index];
}

Node* Tree::taxon(int index)
{
    if (index >= _num_taxa) {
        return NULL;
    }
    return _nodes[index];
}

void Tree::prepStepwise(int left, int right, int anc)
{
    reset();
    
    // Create starting fork
    Node* base = NULL;
    base = newVertex();
    
    base->addDescendant(*_nodes[left]);
    base->addDescendant(*_nodes[right]);
    base->parent(*_nodes[anc]);
    _nodes[anc]->parent(*base);
    _start = base;
    
    // Insert all remaining tips on a base
    int i = 0;
    for (i = 0; i < _num_taxa; ++i)
    {
        if (_nodes[i]->parent() == NULL) {
            base = newVertex();
            base->addDescendant(*_nodes[i]);
        }
    }
   
    // Do traversal
    traverse();
}

/* Private functions */

bool cmpTips(Node* a, Node* b)
{
    return (a->tipNumber() < b->tipNumber());
}

bool cmpInternals(Node* a, Node* b)
{
    return (a->uniqueIndex() < b->uniqueIndex());
}

void Tree::markUniquely(void)
{
    int i = 0;
    int max = _tips.size();
    int index = max;
    std::vector<Node*>::iterator p;
    
    for (p = _nodes.begin(); p != _nodes.end(); ++p) {
        (*p)->_index = 0;
    }
    
    p = _nodes.begin() + _num_taxa;
    std::sort(_tips.begin(), _tips.end(), cmpTips);
    
    Node *q = NULL;
    
    std::cout << "Root's index: " << _start->memIndex();
    _start->_index = _start->memIndex();
    
    for (i = 0; i < max; ++i) {
        std::cout << "For tip " << _tips[i]->tipNumber() << std::endl;
        if (_tips[i]->parent() != NULL) {
            
            q = _tips[i]->parent();
            while (q->parent() != NULL && q->uniqueIndex() == 0)
            {
                std::cout << "Mark: " << index << std::endl;
                q->_index = index;
                q = q->parent();
                ++index;
            }
        }
    }
    
    std::sort(_internals.begin(), _internals.end(), cmpInternals);
}

void Tree::removeBranch(Node &subtr)
{
    Node* base = subtr.parent();
    Node* up = NULL;
    Node* dn = base->parent();
    
    base->popDesc(*up);
    dn->popDesc(*base);
}

void Tree::connectBranch(Node &subtr, Node &tgt)
{
    Node* base = NULL;
    Node* anc = NULL;
    
    if (subtr.parent() == NULL) {
        // TODO: Get a free internal node
    }
    else {
        base = subtr.parent();
    }
    
    anc = tgt.parent();
    anc->popDesc(tgt);
    
    base->addDescendant(tgt);
    anc->addDescendant(*base);
}

void Tree::connectBranch(int subtrIndex, int tgtIndex)
{
    std::cout << "Not implemented\n";
}

/******************************************************************************
 *
 * Private member functions 
 *
 ******************************************************************************/

Node* Tree::markDownpass(int index)
{
    bool found = false;
    
    Node* n = NULL;
    
    _start->markTraverse(index, &found, &n);
    
    return n;
}

void Tree::connectDummy(void)
{
    _dummy_root = _nodes[2 * _num_taxa - 1];
    
    _dummy_root->_tip = -1;
    _dummy_root->_anc = _reserved_root;
    _reserved_root->_anc = _dummy_root;
}

