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
    _branch_lengths.clear();
}

void Topology::store(Tree &t)
{
    clear();
    _natural_score  = t.natScore();
    _real_score     = t.realScore();
    _is_rooted      = t.isrooted();
    
    int i = 0;
    int max = t.size();
    
    for (i = 0; i < max; ++i) {
        if (t._nodes[i]->parent() != NULL) {
            Node* n = NULL;
            n = t._nodes[i]->parent();
            _anc_edges.push_back(n->memIndex());
        }
    }
}
