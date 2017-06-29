//
//  BinNode.cpp
//  Phylogenetic Class Library
//
//  Created by mbrazeau on 28/06/2017.
//  Copyright © 2017 Brazeau Lab. All rights reserved.
//

#include "Node.hpp"
#include "BinNode.hpp"

Node* BinNode::left(void)
{
    if (_left != NULL) {
        return _left;
    }
    
    return NULL;
}

Node* BinNode::right(void)
{
    if (_right != NULL) {
        return _right;
    }
    
    return NULL;
}

void BinNode::addDescendant(Node& desc)
{
    if (_tip != 0) {
        return;
    }
    
    Node** p = NULL;
   
    if (_left == NULL)
    {
        p = &_left;
    }
    else if (_right == NULL)
    {
        p = &_right;
    }
    
    if (p == NULL) {
        // Convert to polynode
    }
    
    desc.parent(*this);
    
}

void BinNode::rotate(void)
{
    
    Node *p = NULL;
    Node *q = NULL;
    Node **r = NULL;
    
    if (_left->isInPath()) {
        p = _left;
        r = &_left;
    }
    else {
        p = _right;
        r = &_right;
    }
    
    q = _anc;
    *r = q;
    
    _anc = p;
    
}
