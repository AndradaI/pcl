//
//  Topology.hpp
//  Phylogenetic Class Library
//
//  Created by mbrazeau on 28/06/2017.
//  Copyright © 2017 Brazeau Lab. All rights reserved.
//

#ifndef Topology_hpp
#define Topology_hpp

#include <vector>
#include <string>

class Tree;

class Topology
{
    int                 _num_tips;
    int                 _num_nodes;
    bool                _is_rooted;
    std::string         _as_Newick;
    std::vector<int>    _node_order;
    std::vector<int>    _anc_edges;
    std::vector<double> _branch_lengths;
    unsigned long       _pars_score;
    double              _like_score;
    
    
public:
    
    
    Topology(int num_taxa, bool isrooted)
    {
        _num_tips = num_taxa;
        _is_rooted = isrooted;
        
        int numnodes = 2 * num_taxa - 1;
        _anc_edges.reserve(numnodes);
        _branch_lengths.reserve(numnodes);
    }
    
    void clear(void);
    void store(Tree& t);
    
};

#endif /* Topology_hpp */
