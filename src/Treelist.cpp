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
    return _topologies.size();
}

unsigned long Treelist::autoIncr(void)
{
    return _auto_increase;
}

void Treelist::autoIncr(unsigned long incr)
{
    _auto_increase = incr;
}

bool Treelist::save(Topology &topol)
{
    _topologies.push_back(&topol);
    return true;
}

Topology* Treelist::getTopol(void)
{
    if (_topologies.size() == 0)
    {
        return NULL;
    }
    
    Topology* ret = NULL;
    ret = _topologies.front();
    _topologies.pop_front();
    
    return ret;
}


void Treelist::clear(void)
{
    _topologies.clear();
}

void Treelist::extend(unsigned long extension)
{
    unsigned long i = 0;
    
    for (i = 0; i < extension; ++i) {
        _topologies.push_back(new Topology(_num_taxa));
    }
}

void Treelist::spliceTopolList(Treelist &treelist)
{
    _topologies.splice(_topologies.begin(), treelist._topologies);
}

/******************************************************************************
 *
 *  Private
 *
 ******************************************************************************/

