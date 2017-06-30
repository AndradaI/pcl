//
//  Node.cpp
//  Phylogenetic Class Library
//
//  Created by mbrazeau on 28/06/2017.
//  Copyright © 2017 Brazeau Lab. All rights reserved.
//

#include "Node.hpp"

#include <iostream>

Node* Node::parent(void)
{
    if (_anc != NULL) {
        return _anc;
    }
    
    return NULL;
}

Node* Node::left(void)
{
    return _left;
}

Node* Node::right()
{
    return _right;
}

int Node::memIndex(void)
{
    return _mem_index;
}

int Node::tipNumber(void)
{
    return _tip;
}

void Node::parent(Node& newparent)
{
    _anc = &newparent;
}

bool Node::isInPath(void)
{
    return _in_path;
}

void Node::disconnectAll(void)
{
    //_descs.assign(_descs.size(), NULL);
    _descs.clear();
    _left = NULL;
    _right = NULL;
    _anc = NULL;
}

void Node::addDescendant(Node &desc)
{
    _descs.push_back(&desc);
    _right = &desc;
    _left = *_descs.begin();
    desc.parent(*this);
}

void Node::traverse
(std::vector<Node *> &inorder,
 std::vector<Node *> &tips,
 std::vector<Node *> &internals)
{
    if (_tip != 0)
    {
        std::cout << _tip;
        inorder.push_back(this);
        tips.push_back(this);
        return;
    }
    
    std::cout << '(';
    
    std::list<Node*>::iterator p;
    p = _descs.begin();
    
    do {
        (*p)->traverse(inorder, tips, internals);
        ++p;
        if (p != _descs.end()) {
            std::cout << ',';
        }
    } while (p != _descs.end());
    
    std::cout << ')';
    
    return;
}

void Node::binTraverse
(std::vector<Node *> &inorder,
 std::vector<Node *> &tips,
 std::vector<Node *> &internals)
{
    if (_tip != 0) {
        std::cout << _tip;
        inorder.push_back(this);
        tips.push_back(this);
        return;
    }
    
    std::cout << '(';
    
    _left->binTraverse(inorder, tips, internals);
    std::cout << ',';
    _right->binTraverse(inorder, tips, internals);
    
    inorder.push_back(this);
    internals.push_back(this);
    
    std::cout << ')';
    return;
}


void Node::rotate(void)
{
    std::list<Node*>::iterator p;
    p = _descs.begin();
   
    while ((*p)->_in_path != true) {
        ++p;
    }
    
    Node* q = NULL;
    Node** r = &(*p);
    
    q = parent();
    *r = q;
    parent(**p); // I think........
}

void Node::markTraverse(int index, bool *found)
{
    if (*found == true) {
        return;
    }
    
    if (_mem_index == index) {
        *found = true;
        _in_path = true;
    }
    
    if (_tip != 0) {
        return;
    }
    
    std::list<Node*>::iterator p;
    p = _descs.begin();
    
    do {
        (*p)->markTraverse(index, found);
        ++p;
        if (p != _descs.end()) {
            std::cout << ',';
        }
    } while (p != _descs.end());
    
    if (*found) {
        _in_path = true;
    }
}

