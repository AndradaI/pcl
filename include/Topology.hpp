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
#include <list>

class Tree;
class Node;

class Topology
{
    unsigned long       _num_taxa;
    bool                _is_rooted;
    std::string         _as_Newick;
    std::vector<int>    _tipnum;
    std::vector<int>    _indices;
    std::vector<int>    _node_order; /*!< Node ordering valid for comparisons & reconstruction. */
    std::vector<int>    _anc_edges; /*!< Edge list valid for reconstruction of tree only. */
    std::vector<double> _branch_lengths;
    std::vector<std::vector<Node*>> _descendants;
    std::vector<Node*>              _parents;
    int                 _start_index;
    unsigned long       _natural_score;
    double              _real_score;
    
public:
    
    Topology(unsigned long num_taxa)
    :   _num_taxa(num_taxa),
        _is_rooted(false),
        _natural_score(0),
        _real_score(0.0)
    {
        unsigned long numnodes = 2 * num_taxa - 1;
        _anc_edges.reserve(numnodes);
        _branch_lengths.reserve(numnodes);
        _node_order.reserve(numnodes);
        _descendants.reserve(numnodes + 1);
        _parents.reserve(numnodes + 1);
    }
    
    void            clear       (void);
    void            store       (Tree& t);
    int             edge        (int index);
    int             index       (int index);
    int             tipnumber   (int index);
    unsigned long   size        (void);
    unsigned long   natScore    (void);
    double          realScore   (void);
    bool            isrooted    (void);
    int             startIndex  (void);
    bool            search      (std::list<Topology*> &topolist);
    bool            checkSame   (Topology& topol);
    
    friend bool     operator==  (const Topology& a, const Topology& b);
    
    friend class Node;
    friend class Tree;
    friend class Treelist;
    
};

#endif /* Topology_hpp */
