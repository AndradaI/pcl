//
//  Treelist.hpp
//  Phylogenetic Class Library
//
//  Created by mbrazeau on 28/06/2017.
//  Copyright © 2017 Brazeau Lab. All rights reserved.
//

#ifndef Treelist_hpp
#define Treelist_hpp

#include <vector>
#include <list>
#include "Topology.hpp"

class Treelist {
    
    unsigned long           _num_trees;
    unsigned long           _max_trees;
    unsigned long           _auto_increase;
    std::list<Topology*>    _saved_trees;
    std::list<Topology*>    _staged_trees;
    std::list<Topology*>    _unused_topols;
    
public:
    
    
};

#endif /* Treelist_hpp */
