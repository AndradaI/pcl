//
//  testTree.cpp
//  Phylogenetic Class Library
//
//  Created by mbrazeau on 30/06/2017.
//  Copyright © 2017 Brazeau Lab. All rights reserved.
//

#include "pcl.h"
#include "TreeTester.hpp"

int test_rerooting(void)
{
    //theader("Testing simple tree rerooting");
    int err     = 0;
    int failn   = 0;
    
    std::cout << "\n";
    std::string testnwk = "((6,((5,2),4)),(1,3));";
    int numtaxa = 6;
    
    NewickReader reader(numtaxa);
    reader.read(testnwk, false, true);
    Topology &topol = reader.getTopol();
    
    Tree t(numtaxa);
    
    t.restore(topol);
    
    TreeTester ttestr;
    ttestr.checkTree(t);
    
    t.root(3);
    ttestr.checkTree(t);
    t.root(4);
    ttestr.checkTree(t);
    t.root(5);
    ttestr.checkTree(t);
    
    t.pruneSubtree(8);
    return failn;
}
