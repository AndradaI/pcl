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

unsigned int Tree::size(void)
{
    return _tips.size() + _internals.size();
}

void Tree::restore(Topology &topol)
{
    std::cout << "Not implemented\n";
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

void Tree::setStart(Node &n)
{
    _start = &n;
}

void Tree::traverse(void)
{
    _nodes.clear();
    _tips.clear();
    _internals.clear();
    _start->traverse(_nodes, _tips, _internals);
}

