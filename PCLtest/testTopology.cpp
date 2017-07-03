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

int test_tree_marking(void)
{
    //    theader("<#testheader#>");
    int err     = 0;
    int failn   = 0;
    
    std::string testnwk1 = "((2,((5,6),4)),(1,3));";
    std::string testnwk2 = "((1,3),(2,(4,(5,6))));";
    std::string testnwk3 = "((4,((5,6),2)),(1,3));";
    std::string testnwk4 = "((3,((1,6),4)),(5,2));";
    int numtaxa = 6;
    
    NewickReader reader(numtaxa);
    reader.read(testnwk1, false, true);
    Topology &topol = reader.getTopol();
    
    Tree t(numtaxa);
    
    t.restore(topol);
    
    TreeTester ttestr;
    ttestr.checkTree(t);
    
    reader.read(testnwk2, false, true);
    topol = reader.getTopol();
    t.restore(topol);
    
    reader.read(testnwk3, false, true);
    topol = reader.getTopol();
    t.restore(topol);
    
    reader.read(testnwk4, false, true);
    topol = reader.getTopol();
    t.restore(topol);
    
    return failn;
}
