//
//  Topology.cpp
//  Phylogenetic Class Library
//
//  Created by mbrazeau on 28/06/2017.
//  Copyright © 2017 Brazeau Lab. All rights reserved.
//

#include "Topology.hpp"

void Topology::clear()
{
    _as_Newick.erase();
    _node_order.clear();
    _anc_edges.clear();
    _branch_lengths.clear();
}
