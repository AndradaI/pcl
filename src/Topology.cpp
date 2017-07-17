//
//  Topology.cpp
//  Phylogenetic Class Library
//
//  Created by mbrazeau on 28/06/2017.
//  Copyright © 2017 Brazeau Lab. All rights reserved.
//

#include "Topology.hpp"
#include "Tree.hpp"

void Topology::clear()
{
    _natural_score  = 0;
    _real_score     = 0.0;
    _as_Newick.erase();
    _node_order.clear();
    _anc_edges.clear();
    _indices.clear();
    _tipnum.clear();
    _branch_lengths.clear();
    _descendants.clear();
    _parents.clear();
    
    int i = 0;
    int max = _anc_edges.capacity();
    for (i = 0; i < max; ++i)
    {
        _anc_edges[i] = 0;
    }
}

void Topology::store(Tree &t)
{
    clear();
    _natural_score  = t.natScore();
    _real_score     = t.realScore();
    _is_rooted      = t.isrooted();
    
    int i = 0;
    int max = static_cast<int>(t.capacity());
    _node_order.resize(max);
    
    t.markUniquely();
    
    int local_ndorder[t.capacity()];
    
    memset(local_ndorder, 0, t.capacity() * sizeof(int));
    
    int j = 0;
    
    //    max = t.size();
    max = t.capacity();
    _indices.resize(max);
    _tipnum.resize(max);
    _anc_edges.resize(max);
    
    for (i = 0; i < max; ++i)
    {
        _indices[i] = t.node(i)->memIndex();
        
        if (t.node(i)->parent() != NULL)
        {
            if (t.node(i)->tipNumber() != 0)
            {
                _node_order[i] = t.node(i)->parent()->uniqueIndex();
            }
            else
            {
                _node_order[t.node(i)->uniqueIndex()] = t.node(i)->parent()->uniqueIndex();
            }
            _anc_edges[i] = t.node(i)->parent()->memIndex() + 1;
        }
        else {
            _anc_edges[i] = 0;
        }
    }
    
    if (t.isrooted() == false) {
        _start_index = t._start->memIndex();
    }
    
#ifdef DEBUG
    std::cout << "Reconstruction order:\n";
    for (i = 0; i < _indices.size(); ++i) {
        std::cout << _indices[i] << " ";
    }
    std::cout << "\n";
    for (i = 0; i < _anc_edges.size(); ++i) {
        std::cout << _anc_edges[i] << " ";
    }
    std::cout << "\n";
    std::cout << "Comparison order:\n";
    for (i = 0; i < _node_order.size(); ++i) {
        std::cout << _node_order[i] << " ";
    }
    std::cout << "\n";
    
    assert(_indices.size() == _anc_edges.size());
#endif
}

int Topology::edge(int index)
{
    return _anc_edges[index];
}

int Topology::index(int index)
{
    return _indices[index];
}

int Topology::tipnumber(int index)
{
    return _tipnum[index];
}

unsigned long Topology::size(void)
{
    return _anc_edges.size();
}

unsigned long Topology::natScore(void)
{
    return _natural_score;
}

double Topology::realScore(void)
{
    return _real_score;
}

bool Topology::isrooted(void)
{
    return _is_rooted;
}

int Topology::startIndex()
{
    return _start_index;
}

bool operator==(const Topology& a, const Topology& b)
{
    return (a._node_order == b._node_order);
}
