//
//  testNode.cpp
//  Phylogenetic Class Library
//
//  Created by mbrazeau on 03/07/2017.
//  Copyright © 2017 Brazeau Lab. All rights reserved.
//

#include "pcl.h"
#include "tests.hpp"
#include "TreeTester.hpp"

int test_node_pop(void)
{
    
    theader("Testing node extraction and reinsertion");
    int err = 0;
    int failn = 0;
    std::string testnwk = "((2,((5,6),4)),(1,3));";
    
    int numtaxa = 6;
    
    NewickReader reader(numtaxa);
    reader.read(testnwk, false, true);
    Topology &topol = reader.getTopol();
    
    TreeTester ttestr;
    
    Tree t(numtaxa);
    
    t.restore(topol);
    
    err = ttestr.checkTree(t);
    if (err) {
        ++failn;
        pfail;
        err = 0;
    }
    else {
        ppass;
    }
    
    Node* np = NULL;
   
    np = t.node(4);
    Node *n1 = np->parent();
    Node *n2 = n1->parent();
    n1->storeDescs();
    n2->storeDescs();
    
    np->removeWithBase();
    
    t.traverse();
    
    ttestr.checkTree(t);
    if (err) {
        ++failn;
        pfail;
        err = 0;
    }
    else {
        ppass;
    }
    
    Node *tgt = t.node(2);
    Node *tgtp = tgt->parent();
    tgtp->storeDescs();
    
    t.connectBranch(*np, *tgt);
    
    t.traverse();
    ttestr.checkTree(t);
    
    if (err) {
        ++failn;
        pfail;
        err = 0;
    }
    else {
        ppass;
    }
    
    n1->restoreDescs();
    n2->restoreDescs();
    tgtp->restoreDescs();
    
    ttestr.checkTree(t);
    if (err) {
        ++failn;
        pfail;
        err = 0;
    }
    else {
        ppass;
    }
    t.traverse();

    Node a_node;
    
    a_node.assignIndex(1);
    
    return failn;
}
