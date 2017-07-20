//
//  Tree.cpp
//  Phylogenetic Class Library
//
//  Created by mbrazeau on 28/06/2017.
//  Copyright © 2017 Brazeau Lab. All rights reserved.
//

#include "Tree.hpp"
#include "Topology.hpp"

bool cmpTips(Node* a, Node* b)
{
    return (a->tipNumber() < b->tipNumber());
}

bool cmpInternals(Node* a, Node* b)
{
    return (a->uniqueIndex() < b->uniqueIndex());
}

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
    //reset();
    int i = 0;
    unsigned long index = 0;
    unsigned long anci = 0;
    unsigned long max = topol.size();
    
    for (i = 0; i < max; ++i) {
        _nodes[i]->clearDescs();
    }
    
    for (i = 0; i < max-1; ++i)
    {
        if (topol.edge(i) != 0) {
            index = topol.index(i);
            anci = topol.edge(i)-1;
            _nodes[anci]->addDescendant(*_nodes[index]);
        }
        
    }
    
    if (topol.isrooted() == true) {
        _start = _reserved_root;
        _isrooted = topol.isrooted();
    }
    else {
        _start = _nodes[topol.startIndex()];
        _isrooted = topol.isrooted();
    }
    
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
        n->clearDescs();
        n->clearStoredDescs();
        n->unmarkClipSite();
        if (n->tipNumber() != 0)
        {
            _free_tips.push_back(n);
        }
        else if (n != _reserved_root && n != _dummy_root){
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
    //_nextFreeTip = _nodes.begin();
    //_nextFreeInternal = _nodes.begin() + _num_taxa;
}

void Tree::incrScore(unsigned long s)
{
    _natural_score += s;
}

void Tree::incrScore(double s)
{
    _real_score += s;
}

void Tree::setNatScore(unsigned long s)
{
    _natural_score = s;
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
    Node *ret = _nodes[id_number];
    //Node *base = newVertex();
    //base->addDescendant(*ret);
    
    return ret;
}


Node* Tree::newVertex(void)
{
    if (_free_vertices.size() != 0)
    {
        Node *ret = _free_vertices.back();
        _free_vertices.pop_back();
        ret->disconnectAll();
        //ret->_descs.clear();
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
    else if (_start == _reserved_root)
    {
        _isrooted = true;
        return;
    }
    
    Node* p = NULL;
    Node* q = NULL;
    
    p = _start;
    q = _start->left();
    
    p->disconnectAll();
    q->_anc = NULL;
    
    _reserved_root->_descs.clear();
    _reserved_root->addDescendant(*p);
    _reserved_root->addDescendant(*q);
    
    _reserved_root->parent(*_dummy_root);
    _start = _reserved_root;
    
    _isrooted = true;
    traverse();
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
    int index = _starttip;
    
    //index = _tips[_starttip]->memIndex(); // Get the lowest tip in the tree
    
    root(index);
    
    Node* tipstart = _nodes[_starttip];
    tipstart->disconnectAll();
    
    _reserved_root->popDesc(*tipstart);
    
    Node* instart = _reserved_root->left();
    
    instart->_anc = NULL;
    
    tipstart->addDescendant(*instart);
    instart->_anc = tipstart;
    
    _reserved_root->_descs.clear();
    _reserved_root->_left = NULL;
    _reserved_root->_right = NULL;
    
    _start = tipstart;
    
    _isrooted = false;
    //traverse();
}

void Tree::traverse(void)
{
    _postorder.clear();
    _tips.clear();
    _internals.clear();
    
    if (_isrooted == true) {
        _start->traverse(_postorder, _tips, _internals);
    } else {
        _start->left()->traverse(_postorder, _tips, _internals);
        _postorder.push_back(_start);
        _tips.push_back(_start);
    }
    
    std::sort(_tips.begin(), _tips.end(), cmpTips);
    std::cout << "\n";
}

void Tree::traverse(std::vector<Node *> &inorder)
{
    if (inorder.capacity() < capacity())
    {
        inorder.resize(capacity());
    }
    
    inorder.clear();
    
    if (_isrooted == true) {
        _start->traverse(inorder, _tips, _internals);
    } else {
        _start->left()->traverse(inorder, _tips, _internals);
        inorder.push_back(_start);
        _tips.push_back(_start);
    }
    std::cout << "\n";
}

std::string Tree::writeNewick(void)
{
    bool unrooted = false;
    std::string Newick;
    Newick.reserve(_num_taxa * 2);
    Newick.clear();
    if (_isrooted) {
        Newick.append("[&R] ");
    } else {
        Newick.append("[&U] ");
        unrooted = true;
        root();
    }
    
    _start->writeNewick(Newick);
    
    if (unrooted)
    {
        unroot();
    }
    
    Newick.push_back(';');
    
    return Newick;
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
    _nodes[anc]->addDescendant(*base);
    _nodes[anc]->_anc = NULL;
    _start = _nodes[anc];
    _isrooted = false;
    
    // Insert all remaining tips on a base
    int i = 0;
    for (i = 0; i < _num_taxa; ++i)
    {
        if (i != left && i != right && i != anc) {
            base = newVertex();
            base->addDescendant(*_nodes[i]);
        }
    }
    
    _starttip = anc;
   
    // Do traversal
    traverse();
}

void Tree::markUniquely(void)
{
    int i = 0;
    
    // TODO: Make safer
    unsigned long max = _tips.size();
    int index = (int)max;
    std::vector<Node*>::iterator p;
    
    for (p = _nodes.begin(); p != _nodes.end(); ++p) {
        (*p)->_index = 0;
    }
    
    Node *q = NULL;
    
    for (i = 0; i < max; ++i)
    {
        q = NULL;
        q = _tips[i]->parent();
        
        if (q != NULL)
        {
            while (q && q->uniqueIndex() == 0 && q->tipNumber() >= 0)
            {
                q->_index = index;
                q = q->parent();
                ++index;
            }
        }
    }
}

void Tree::removeBranch(Node &subtr)
{
    Node* base = subtr.parent();
    Node* up = NULL;
    Node* dn = base->parent();
    
    base->popDesc(*up);
    dn->popDesc(*base);
    base->_anc = NULL;
}

void Tree::connectBranch(Node &subtr, Node &tgt)
{
    Node* base = NULL;
    Node* anc = NULL;
    
    if (subtr.parent() == NULL) {
        // TODO: Get a free internal node
        std::cout << "Warning! Failing to base subtree\n";
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
    // std::cout << "Not implemented\n";
}

void Tree::tempInsert(Node& src, Node& tgt)
{
    src.parent()->storeDescs();
    tgt.parent()->storeDescs();
    connectBranch(src, tgt);
}

void Tree::undoTempInsert(Node& src)
{
    src.parent()->parent()->restoreDescs();
    src.parent()->restoreDescs();
    src.parent()->_anc = NULL;
}

void Tree::putInOutgroup(int index)
{
    _nodes[index]->outgroup();
}

void Tree::putInIngroup(int index)
{
    _nodes[index]->ingroup();
}

void Tree::doBreakList(std::vector<Node *> &breaklist)
{
    breaklist.clear();
    int max_subtr_size = 0;
    max_subtr_size = (int)size() - 3;
    
    if(isrooted() == true)
    {
        _start->travBreakList(breaklist, max_subtr_size);
    }
    else
    {
        _start->left()->travBreakList(breaklist, max_subtr_size);
    }
}

void Tree::doReconnectList(std::vector<Node *> &reconnectlist)
{
    reconnectlist.clear();
    
    if (isrooted() == true)
    {
        _start->travReconnectList(reconnectlist);
    }
    else
    {
        _start->left()->travReconnectList(reconnectlist);
    }
}

void Tree::doRerootList(std::vector<Node *> &rerootlist)
{
    rerootlist.clear();
    
    if (_start->tipNumber() == 0)
    {
        unsigned long firstsize = 0;
        if (_start->left()->_weight > 1)
        {
            _start->left()->traverse(rerootlist, _tips, _internals);
            if (rerootlist.size() > 0) {
                rerootlist.pop_back();
            }
            firstsize = rerootlist.size();
        }
        if (_start->right()->_weight > 1)
        {
            _start->right()->traverse(rerootlist, _tips, _internals);
            //            _start->right()->travReconnectList(rerootlist);
            if (rerootlist.size() > firstsize) {
                rerootlist.pop_back();
            }
        }
    }
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
    _dummy_root->_anc = NULL;
    _dummy_root->addDescendant(*_reserved_root);
}

