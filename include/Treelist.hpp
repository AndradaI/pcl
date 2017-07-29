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
    
    unsigned long                   _num_taxa;
    unsigned long                   _max_trees;
    unsigned long                   _auto_increase;
    std::list<Topology*>            _topolbuffer;
    std::list<Topology*>            _free_topols;
    std::list<Topology*>::iterator  _next_topol;
    
public:
    
    Treelist(unsigned long numtaxa, unsigned long numtrees, unsigned long autoincr)
    :   _num_taxa(numtaxa),
        _auto_increase(autoincr)
    {
        if (numtrees != 0)
        {
            extend(numtrees);
        }
        _next_topol = _topolbuffer.begin();
    }
    
    unsigned long   numTrees            (void);
    unsigned long   autoIncr            (void);
    void            autoIncr            (unsigned long incr);
    unsigned long   maxTrees            (void);
    void            maxTrees            (unsigned long maxtrees);
    bool            save                (Topology& topol);
    bool            save                (Tree& t);
    Topology*       getTopol            (void);                 /*!< Get a topology */
    void            clear               (void);
    void            clearBuffer         (void);
    bool            checkNew            (Topology& newTopol);
    bool            checkNew            (Tree& t);
    bool            checkNewFromCurrent (Topology& newTopol);
    void            pushList            (std::list<Topology*>& exlist);
    
    void            spliceTopolList     (Treelist& treelist);
    void            extend              (unsigned long extension);
    Topology*       getNext             (void);
    void            resetTopolIterator  (void);
    void            incrNext            (void);
    unsigned long   getBestNatScore     (void);
    void            keepBest            (void);
    void            removeDuplicates    (void);
    
    friend bool     operator& (Treelist& a, Treelist& b);
    
private:
    
    Topology*       getFreeTopology     (void);
    
};

#endif /* Treelist_hpp */
