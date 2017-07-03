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
    
    unsigned long           _num_taxa;
    unsigned long           _num_trees;
    unsigned long           _max_trees;
    unsigned long           _auto_increase;
    std::list<Topology*>    _saved_trees;
    
public:
    
    Treelist(unsigned long numtaxa, unsigned long numtrees, unsigned long maxtrees, unsigned long autoincr)
    :   _num_taxa(numtaxa),
        _num_trees(numtrees),
        _max_trees(maxtrees),
        _auto_increase(autoincr)
    {
        int i = 0;
        for (i = 0; i < numtrees; ++i) {
            _saved_trees.push_back(new Topology(numtaxa));
        }
    }
    
    unsigned long   numTrees    (void);
    unsigned long   maxTrees    (void);
    unsigned long   autoIncr    (void);
    void            autoIncr    (unsigned long incr);
    bool            push        (Topology& topol);
    Topology*       pop         (void); /*!< Get a topology */
    void            clearSaved  (void);
    bool            extend      (unsigned long s);
    bool            checkNew    (Topology& newTopol); /*!< Checks whether proposed topology is not found in list */
    
};

#endif /* Treelist_hpp */
