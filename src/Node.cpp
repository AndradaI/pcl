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

void Node::parent(Node& newparent)
{
    _anc = &newparent;
}

void Node::assignIndex(unsigned long index)
{
    _index = index;
}

int Node::parentIndex(void)
{
    return _anc->memIndex();
}

bool Node::isPolynode(void)
{
    if (_descs.size() > 2) {
        return true;
    }
    
    return false;
}

Node* Node::left(void)
{
    return _left;
}

int Node::leftIndex(void)
{
    return _left->memIndex();
}

Node* Node::right()
{
    return _right;
}

int Node::rightIndex(void)
{
    return _right->memIndex();
}

int Node::memIndex(void)
{
    return _mem_index;
}

int Node::tipNumber(void)
{
    return _tip;
}

int Node::uniqueIndex(void)
{
    return _index;
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
    _right = _descs.back();
    _left = _descs.front();
    desc.parent(*this);
}

int Node::traverse
(std::vector<Node *> &inorder,
 std::vector<Node *> &tips,
 std::vector<Node *> &internals)
{
    if (_tip != 0)
    {
        std::cout << _tip;
        inorder.push_back(this);
        tips.push_back(this);
        return 1;
    }
    
    int weight = 0;
    std::cout << '(';
    
    std::vector<Node*>::iterator p;
    p = _descs.begin();
    
    do {
        weight += (*p)->traverse(inorder, tips, internals);
        ++p;
        if (p != _descs.end()) {
            std::cout << ',';
        }
    } while (p != _descs.end());
    
    std::cout << ')';
    
    inorder.push_back(this);
    internals.push_back(this);
    
    _weight = weight;
    
    return weight;
}

int Node::binTraverse
(std::vector<Node *> &inorder,
 std::vector<Node *> &tips,
 std::vector<Node *> &internals)
{
    if (_tip != 0) {
        std::cout << _tip;
        inorder.push_back(this);
        tips.push_back(this);
        return 1;
    }
    
    int weight = 0;
    std::cout << '(';
    
    weight += _left->binTraverse(inorder, tips, internals);
    std::cout << ',';
    weight += _right->binTraverse(inorder, tips, internals);
    
    std::cout << ')';
    
    inorder.push_back(this);
    internals.push_back(this);
    
    _weight = weight;
    
    return weight;
}


void Node::removeWithBase(void)
{
    Node* base = parent();
    
    if (base->_descs.size() > 2)
    {
        std::cout << "ERROR: Extraction on non-binary node\n";
        // TODO: Might call the node resolver when that is written
        return;
    }
    
    std::vector<Node*>::iterator q = base->_descs.begin();
    
    while (*q == this) ++q;
    
    Node* dn = base->parent();
    Node* up = *q;
    
    base->popDesc(*up);
    dn->popDesc(*base);
    
    dn->addDescendant(*up);
    
}

void Node::storeDescs(void)
{
    _storeddescs.clear();
    _storeddescs = _descs;
}

void Node::restoreDescs(void)
{
    std::vector<Node*>::iterator p;
    
    _descs.clear();
    
    for (p = _storeddescs.begin(); p != _storeddescs.end(); ++p)
    {
        addDescendant(**p);
    }
    
    _left = _descs.front();
    _right = _descs.back();
    clearStoredDescs();
}

void Node::clearStoredDescs(void)
{
    _storeddescs.clear();
}


/******************************************************************************
 *                                                                            *
 *  Protected member functions                                                *
 *                                                                            *
 ******************************************************************************/

void Node::rotate(void)
{
/*    if (_tip != 0) {
        return;
    }*/
    
    if (!_left->isInPath() && !_right->isInPath())
    {
        return;
    }
    
    Node* p = NULL;
   
    std::vector<Node*>::iterator i;
    i = _descs.begin();
    
    while ((*i)->_in_path != true)
    {
        ++i;
    }
    
    p = *i;
    _descs.erase(i);
    _descs.push_back(_anc);
    _left = _descs.front();
    _right = _descs.back();
    
    _anc = p;
}

void Node::markTraverse(int index, bool *found, Node** n)
{
    if (*found == true)
    {
        return;
    }
    
    if (_mem_index == index)
    {
        std::cout << "Found node: " << _mem_index << " tip number: " << _tip << std::endl;
        *found = true;
        *n = this;
        _in_path = true;
    }
    
    if (_tip != 0) {
        return;
    }
    
    std::vector<Node*>::iterator p;
    p = _descs.begin();
    
    do
    {
        (*p)->markTraverse(index, found, n);
        ++p;
    }
    while (p != _descs.end());
    
    if (*found) {
        _in_path = true;
    }
}

/******************************************************************************
 *
 * Private member functions
 *
 ******************************************************************************/

void Node::popDesc(Node &desc)
{
    std::vector<Node*>::iterator p = _descs.begin();
    
    while (*p != &desc) ++p;
    
    (*p)->_anc = NULL;
    
    _descs.erase(p);
    
    _left = *_descs.begin();
    _right = _descs.back();
}


