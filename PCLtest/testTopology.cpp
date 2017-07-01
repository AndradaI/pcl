//
//  testTopology.cpp
//  Phylogenetic Class Library
//
//  Created by mbrazeau on 30/06/2017.
//  Copyright © 2017 Brazeau Lab. All rights reserved.
//

#include "pcl.h"
#include "TreeTester.hpp"

int test_topology_construction(void)
{
    //theader("");
    int err     = 0;
    int failn   = 0;

    std::string testnwk = "((2,((5,6),4)),(1,3));";
    int numtaxa = 6;
    
    NewickReader reader(numtaxa);
    reader.read(testnwk, false, true);
    Topology &topol = reader.getTopol();
    
    Tree t(numtaxa);
    
    t.restore(topol);
    
    TreeTester ttestr;
    
    ttestr.checkTree(t);
    
    return failn;
}
