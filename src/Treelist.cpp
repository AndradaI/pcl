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
    return _topolbuffer.size();
}

unsigned long Treelist::autoIncr(void)
{
    return _auto_increase;
}

unsigned long Treelist::maxTrees(void)
{
    return _max_trees;
}

void Treelist::maxTrees(unsigned long maxtrees)
{
    _max_trees = maxtrees;
}

void Treelist::autoIncr(unsigned long incr)
{
    _auto_increase = incr;
}

bool Treelist::save(Topology &topol)
{
    _topolbuffer.push_back(&topol);
    return true;
}

bool Treelist::save(Tree &t)
{
    bool res = false;
    
    if (_topolbuffer.size() == _max_trees)
    {
        if (_auto_increase == 0)
        {
            return res;
        }
        else
        {
            int i = 0;
            for (i = 0; i < _auto_increase; ++i)
            {
                _free_topols.push_back(new Topology(_num_taxa));
            }
        }
    }
    
    Topology* save = NULL;
    
    save = getFreeTopology();
    save->store(t);
    _topolbuffer.push_back(save);
    
    if (_topolbuffer.size() == 1)
    {
        _next_topol = _topolbuffer.begin();
    }
    else if (_next_topol == _topolbuffer.end())
    {
        --_next_topol;
    }
    
    return res;
}

Topology* Treelist::getTopol(void)
{
    if (_topolbuffer.size() == 0)
    {
        return NULL;
    }
    
    Topology* ret = NULL;
    ret = _topolbuffer.front();
    _topolbuffer.pop_front();
    
    return ret;
}

void Treelist::clear(void)
{
    _topolbuffer.clear();
}

void Treelist::clearBuffer(void)
{
    _free_topols.splice(_free_topols.end(), _topolbuffer);
    _topolbuffer.clear(); // TODO: This might not be necessary
    //_next_topol = _topolbuffer.begin();
}

/* Checks whether proposed topology is not found in list */
bool Treelist::checkNew(Topology &newTopol)
{
    return newTopol.search(_topolbuffer);
}

bool Treelist::checkNew(Tree &t)
{
    // TODO: Can be optimised by using an existing topology object.
    Topology topol(_num_taxa);
    topol.store(t);
    return checkNew(topol);
}

bool Treelist::checkNewFromCurrent(Topology &newTopol)
{
    std::list<Topology*>::iterator s = _next_topol;
    
    while (s != _topolbuffer.end())
    {
        if (newTopol == **s)
        {
            return true;
        }
        ++s;
    }
    
    return false;
}

void Treelist::extend(unsigned long extension)
{
    unsigned long i = 0;
    
    for (i = 0; i < extension; ++i)
    {
        _topolbuffer.push_back(new Topology(_num_taxa));
    }
}

void Treelist::pushList(std::list<Topology*> &exlist)
{
    _topolbuffer.splice(_topolbuffer.begin(), exlist);
}

void Treelist::spliceTopolList(Treelist &treelist)
{
    unsigned long oldbufsize = treelist.numTrees();
    
    if (_topolbuffer.size() == 0)
    {
        _topolbuffer.splice(_topolbuffer.end(), treelist._topolbuffer);
        _next_topol = _topolbuffer.begin();
    }
    else {
        _topolbuffer.splice(_topolbuffer.end(), treelist._topolbuffer);
    }
    
}

Topology* Treelist::getNext(void)
{
    if (_next_topol == _topolbuffer.end())
    {
        return NULL;
    }
    
    Topology* ret = *_next_topol;
    ++_next_topol;
    
    return ret;
}

void Treelist::resetTopolIterator(void)
{
    _next_topol = _topolbuffer.begin();
}

unsigned long Treelist::getBestNatScore(void)
{
    unsigned long bestscore = 0;
    
    bestscore = _topolbuffer.front()->natScore();
    
    std::list<Topology*>::iterator t = _topolbuffer.begin();
    
    while (t != _topolbuffer.end())
    {
        if ((*t)->natScore() < bestscore)
        {
            bestscore = (*t)->natScore();
        }
        ++t;
    }
    
    return bestscore;
}

void Treelist::keepBest(void)
{
    unsigned long best = getBestNatScore();
    
    std::list<Topology*>::iterator t = _topolbuffer.begin();
    while (t != _topolbuffer.end())
    {
        if ((*t)->natScore() > best)
        {
            _free_topols.push_back(*t);
            _topolbuffer.erase(t);
        }
        ++t;
    }
}

void Treelist::removeDuplicates(void)
{
    std::list<Topology*>::iterator t;
    std::list<Topology*>::iterator u;
    
    t = _topolbuffer.begin();
    
    while (t != _topolbuffer.end())
    {
        u = t;
        
        do {
            std::list<Topology*>::iterator v;
            ++u;
            if (**u == **t)
            {
                v = --u;
                _free_topols.push_back(*u);
                _topolbuffer.erase(u);
                u = v;
            }
        } while (u != _topolbuffer.end());
        ++t;
    }
}


bool operator& (Treelist& a, Treelist& b)
{
    std::list<Topology*>::iterator top = a._topolbuffer.begin();
    
    for (; top != a._topolbuffer.end(); ++top)
    {
        if (b.checkNew(**top))
        {
            return true;
        }
    }
    
    return false;
}

/******************************************************************************
 *                                                                            *
 *  Private member functions                                                  *
 *                                                                            *
 ******************************************************************************/

Topology* Treelist::getFreeTopology(void)
{
    Topology* ret = NULL;
    
    if (_free_topols.size() > 0)
    {
        ret = _free_topols.front();
    }
    
    if (ret == NULL)
    {
        ret = new Topology(_num_taxa);
    }
    else {
        _free_topols.pop_front();
    }
    
    return ret;
}

