//
//  Subtree.hpp
//  Phylogenetic Class Library
//
//  Created by mbrazeau on 29/06/2017.
//  Copyright © 2017 Brazeau Lab. All rights reserved.
//

#ifndef Subtree_hpp
#define Subtree_hpp

#include "Tree.hpp"

class Subtree : public Tree {
    
    
    
public:
    
    Subtree(Node& n)
    {
        _start = &n;
        this->traverse();
    }
    
};

#endif /* Subtree_hpp */
