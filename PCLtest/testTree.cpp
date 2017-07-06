//
//  testTree.cpp
//  Phylogenetic Class Library
//
//  Created by mbrazeau on 30/06/2017.
//  Copyright © 2017 Brazeau Lab. All rights reserved.
//

#include "pcl.h"
#include "TreeTester.hpp"

#include <numeric>

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
    t.root(5);
    ttestr.checkTree(t);
    t.root(6);
    ttestr.checkTree(t);
    t.root(7);
    ttestr.checkTree(t);
    t.root(8);
    ttestr.checkTree(t);
    t.root(9);
    ttestr.checkTree(t);
    
    return failn;
}

int test_subtrees_and_rerooting(void)
{
    //theader("<#testheader#>");
    int err     = 0;
    int failn   = 0;
    
    std::string testnwk = "(((1,5),(3,9)),((((2,4),6),(7,8)),10));";
    int numtaxa = 10;
    
    NewickReader reader(numtaxa);
    reader.read(testnwk, false, true);
    Topology &topol = reader.getTopol();
    
    Tree t(numtaxa);
    
    t.restore(topol);
    
    TreeTester ttestr;
    ttestr.checkTree(t);
    
    Subtree subtr(numtaxa);
    
    Node* node = t.postorder(6);
    
    subtr.init(*node);
    
    Node* target = subtr.postorder(0);
    subtr.root(*target);
    
    std::cout<<"\n";
    t.traverse();
    ttestr.checkTree(t);
    return failn;
}

int test_stepwise_addition(void)
{
    int err = 0;
    int failn = 0;
    
    int ntax = 6;
    
    std::vector<int> tipnums(ntax);
    std::iota(tipnums.begin(), tipnums.end(), 0);
    
    Tree tree(ntax);
    
    tree.prepStepwise(1, 2, 0);
    
    Node* newtip = NULL;
    Node* target = NULL;
    
    newtip = tree.taxon(3);
    target = tree.postorder(0);
    
    tree.connectBranch(*newtip, *target);
    tree.traverse();
    
    newtip = tree.taxon(4);
    target = tree.postorder(3);
    
    tree.connectBranch(*newtip, *target);
    tree.traverse();
    
    newtip = tree.taxon(5);
    target = tree.postorder(0);
    
    tree.connectBranch(*newtip, *target);
    tree.traverse();
    
    tree.root();
    tree.traverse();
    
    TreeTester ttestr;
    ttestr.checkTree(tree);
    
    return failn;
    
}
