//
//  Treelist.cpp
//  Phylogenetic Class Library
//
//  Created by mbrazeau on 28/06/2017.
//  Copyright © 2017 Brazeau Lab. All rights reserved.
//

#include "Treelist.hpp"

unsigned long Treelist::numTrees(void)
{
    return _num_trees;
}

unsigned long Treelist::maxTrees(void)
{
    return _max_trees;
}

unsigned long Treelist::autoIncr(void)
{
    return _auto_increase;
}

void Treelist::autoIncr(unsigned long incr)
{
    _auto_increase = incr;
}

bool Treelist::push(Topology &topol)
{
    if (_saved_trees.size() >= _max_trees)
    {
        if (_auto_increase != 0) {
            _max_trees += _auto_increase;
        }
        else {
            return false;
        }
    }
    
    _saved_trees.push_back(&topol);
    
    return true;
}

Topology* Treelist::pop(void)
{
    if (_saved_trees.size() == 0) {
        return NULL;
    }
    
    Topology* ret = _saved_trees.front();
    _saved_trees.pop_front();
    --_num_trees;
    assert(_num_trees == _saved_trees.size());
    return ret;
}
