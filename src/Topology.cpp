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
    _tipnum.clear();
    _branch_lengths.clear();
}

void Topology::store(Tree &t)
{
    clear();
    _natural_score  = t.natScore();
    _real_score     = t.realScore();
    _is_rooted      = t.isrooted();
    
    int i = 0;
    int max = static_cast<int>(t.capacity()-1);
    
    t.markUniquely();
    
    for (i = 0; i <= max; ++i)
    {
        if (t._nodes[i]->parent() != NULL)
        {
            
            if (t._nodes[i]->tipNumber() != 0) {
                _node_order[t._nodes[i]->tipNumber() - 1] = t._nodes[i]->parent()->uniqueIndex();
            }
            else {
                _node_order[t._nodes[i]->memIndex()] = t._nodes[i]->parent()->uniqueIndex();
            }
            //_node_order.push_back(t._nodes[i]->parent()->uniqueIndex());
            _indices.push_back(t._nodes[i]->memIndex());
            _tipnum.push_back(t._nodes[i]->tipNumber());
            Node* n = NULL;
            n = t._nodes[i]->parent();
            _anc_edges.push_back(n->memIndex());
        }
    }
    
    assert(_indices.size() == _anc_edges.size());
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

bool operator==(const Topology& a, const Topology& b)
{
    return (a._node_order == b._node_order);
}
