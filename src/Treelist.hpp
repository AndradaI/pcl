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
#include <algorithm>
#include "Topology.hpp"

class Treelist
{
    
    unsigned long           _num_taxa;
    unsigned long           _auto_increase;
    std::list<Topology*>    _topologies;
    
public:
    
    Treelist(unsigned long numtaxa, unsigned long numtrees, unsigned long autoincr)
    :   _num_taxa(numtaxa),
        _auto_increase(autoincr)
    {
        if (numtrees != 0) {
            extend(numtrees);
        }
    }
    
    unsigned long   numTrees            (void);
    unsigned long   autoIncr            (void);
    void            autoIncr            (unsigned long incr);
    bool            save                (Topology& topol);
    Topology*       getTopol            (void);                 /*!< Get a topology */
    void            clear               (void);
    bool            checkNew            (Topology& newTopol);   /*!< Checks whether proposed topology is not found in list */
    void            pushList            (std::list<Topology*>& exlist);
    
    void            spliceTopolList     (Treelist& treelist);
    void            extend              (unsigned long extension);
    
};

#endif /* Treelist_hpp */
