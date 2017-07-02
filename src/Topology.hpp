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
#include <assert.h>

class Tree;

class Topology
{
    
    bool                _is_rooted;
    std::string         _as_Newick;
    std::vector<int>    _tipnum;
    std::vector<int>    _indices;
    std::vector<int>    _node_order;
    std::vector<int>    _anc_edges;
    std::vector<double> _branch_lengths;
    unsigned long       _natural_score;
    double              _real_score;
    
public:
    
    
    Topology(int num_taxa, bool isrooted)
    {
        _is_rooted = isrooted;
        
        int numnodes = 2 * num_taxa - 1;
        _anc_edges.reserve(numnodes);
        _branch_lengths.reserve(numnodes);
    }
    
    void            clear(void);
    void            store(Tree& t);
    int             edge(int index);
    int             index(int index);
    int             tipnumber(int index);
    unsigned long   size(void);
    
    friend bool     operator==(const Topology& a, const Topology& b);
    
};

#endif /* Topology_hpp */
