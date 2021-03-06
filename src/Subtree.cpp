//
//  Subtree.cpp
//  Phylogenetic Class Library
//
//  Created by mbrazeau on 29/06/2017.
//  Copyright © 2017 Brazeau Lab. All rights reserved.
//

#include "Subtree.hpp"

void Subtree::init(Node &n)
{
    //reset();
    _start = &n;
    _isrooted = true;
    _reserved_root = &n;
    this->traverse();
}

Node* Subtree::rootNode(void)
{
    return _start;
}

Node& Subtree::clip(void)
{
    Node* base      = _start->parent();
    Node* dn        = base->parent();
    Node* up        = _start->sibling();
    _orig_sibling   = up;
    Node* ret       = NULL;
    
    if (base->tipNumber() == 0)
    {
        up->markClipSite();
        base->parent()->markClipSite();
        
        _oldp_descs.clear();
        _oldchild = base;
        _oldp_descs = dn->_descs;
        _oldc_descs.clear();
        _oldc_descs = base->_descs;
        _oldparent = dn;
        _start->removeWithBase();
        
        assert(up->parent() == dn);
        ret = up;
    }
    else {
        ret = base;
        _oldparent = base;
        _oldparent->clearDescs();
        _start->_anc = NULL;
    }
    
    
    return *ret;
}

void Subtree::reconnect(void)
{
    if (_start->_anc != NULL)
    {
        _oldparent->restoreDescs(_oldp_descs);
        _oldchild->restoreDescs(_oldc_descs);
        _start->sibling()->unmarkClipSite();
        _oldparent->unmarkClipSite();
        _orig_sibling = NULL;
    }
    else {
        _oldparent->disconnectAll();
        _oldparent->addDescendant(*_start);
    }
}

void Subtree::root(int index)
{
    if (_start->_weight < 3) {
        return;
    }
    
    Tree::root(index);
}

void Subtree::root(Node &n)
{
    
    if (internalStart()._weight < 2) {
        return;
    }
    
    Tree::root(n);
}

Node* Subtree::origSibling(void)
{
    return _orig_sibling;
}
