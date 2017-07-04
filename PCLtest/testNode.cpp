//
//  testNode.cpp
//  Phylogenetic Class Library
//
//  Created by mbrazeau on 03/07/2017.
//  Copyright © 2017 Brazeau Lab. All rights reserved.
//

#include "pcl.h"
#include "TreeTester.hpp"

int test_node_pop(void)
{
    
    int failn = 0;
    std::string testnwk = "((2,((5,6),4)),(1,3));";
    
    int numtaxa = 6;
    
    NewickReader reader(numtaxa);
    reader.read(testnwk, false, true);
    Topology &topol = reader.getTopol();
    
    Tree t(numtaxa);
    
    t.restore(topol);
    
    Node* np = NULL;
   
    np = t.node(4);
    Node *n1 = np->parent();
    Node *n2 = n1->parent();
    n1->storeDescs();
    n2->storeDescs();
    
    np->removeWithBase();
    
    t.traverse();
    
    TreeTester ttestr;
    ttestr.checkTree(t);
    
    Node *tgt = t.node(2);
    Node *tgtp = tgt->parent();
    tgtp->storeDescs();
    
    t.connectBranch(*np, *tgt);
    
    t.traverse();
    ttestr.checkTree(t);
    
    
    n1->restoreDescs();
    n2->restoreDescs();
    tgtp->restoreDescs();
    
    ttestr.checkTree(t);
    t.traverse();

    
    return failn;
}
