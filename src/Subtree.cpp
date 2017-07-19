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
    this->traverse();
}

Node* Subtree::rootNode(void)
{
    return _start;
}

void Subtree::clip(void)
{
    Node* base = _start->parent();
    Node* dn   = base->parent();
    dn->markClipSite();
    
    //_oldp_descs.clear();
    _oldchild = base;
    _oldp_descs = dn->_descs;
    //_oldc_descs.clear();
    _oldc_descs = base->_descs;
    _oldparent = dn;
    
    _start->removeWithBase();
}

void Subtree::reconnect(void)
{
    _oldparent->restoreDescs(_oldp_descs);
    _oldparent->unmarkClipSite();
    _oldchild->restoreDescs(_oldc_descs);
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
    
    if (_start->_weight < 3) {
        return;
    }
    
    Tree::root(n);
}
