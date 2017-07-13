//
//  testTree.cpp
//  Phylogenetic Class Library
//
//  Created by mbrazeau on 30/06/2017.
//  Copyright © 2017 Brazeau Lab. All rights reserved.
//

#include "pcl.h"
#include "tests.hpp"
#include "TreeTester.hpp"

#include <numeric>

int test_rerooting(void)
{
    theader("Testing simple tree rerooting");
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

int test_unrooted_trees(void)
{
    theader("Testing unrooted trees");
    int err     = 0;
    int failn   = 0;
    
    int ntax = 10;
    std::string newicktree = "(((1,7),10),(((2,((3,6),9)),(5,8)),4));";
    
    NewickReader nwkreader(ntax);
    Topology* topol = NULL;
    Tree tree(ntax);
    
    nwkreader.read(newicktree, false, true);
    topol = &nwkreader.getTopol();
    tree.restore(*topol);
    
    tree.unroot();
    
    TreeTester treetester;
    
    treetester.checkTree(tree);
    
    return failn;
}

int test_subtrees_and_rerooting(void)
{
    theader("Testing rerooting of a subtree");
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
    theader("Testing a crude stepwise addition of branches");
    int err = 0;
    int failn = 0;
    
    int ntax = 6;
    
    std::vector<int> tipnums(ntax);
    std::iota(tipnums.begin(), tipnums.end(), 0);
    
    Tree tree(ntax);
    TreeTester treetester;
    
    tree.prepStepwise(1, 2, 0);
    
    Node* newtip = NULL;
    Node* target = NULL;
    
    newtip = tree.taxon(3);
    target = tree.postorder(0);
    
    tree.connectBranch(*newtip, *target);
    tree.traverse();
    
    treetester.checkTree(tree);
    
    newtip = tree.taxon(4);
    target = tree.postorder(0);
    
    tree.connectBranch(*newtip, *target);
    tree.traverse();
    
    treetester.checkTree(tree);
    
    newtip = tree.taxon(5);
    target = tree.postorder(0);
    
    tree.connectBranch(*newtip, *target);
    tree.traverse();
    
    treetester.checkTree(tree);
    
    tree.root();
    tree.traverse();
    
    TreeTester ttestr;
    ttestr.checkTree(tree);
    
    return failn;
    
}

int test_temp_insert_and_restore(void)
{
    theader("Testing temporary insertion and removal (restore)");
    int err     = 0;
    int failn   = 0;

    
    std::string testnwk = "((2,((5,6),4)),(1,3));";
    
    int numtaxa = 6;
    
    NewickReader reader(numtaxa);
    reader.read(testnwk, false, true);
    Topology &topol = reader.getTopol();
    
    Tree t(numtaxa);
    
    t.restore(topol);
    
    Topology *state1 = new Topology(numtaxa);
    
    Node* src = t.node(4);
    Node* tgt = t.node(5);
    
    src->removeWithBase();
    t.traverse();
    
    state1->store(t);
    
    t.tempInsert(*src, *tgt);
    t.traverse();
    
    t.undoTempInsert(*src);
    t.traverse();
    
    Topology *state2 = new Topology(numtaxa);
    state2->store(t);
    
    
    TreeTester ttestr;
    ttestr.checkTree(t);
   
    if (!(*state2 == *state1)) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    src = t.node(5)->parent();
    tgt = t.node(1);
    
    src->removeWithBase();
    t.traverse();
    
    state1->store(t);
    
    t.tempInsert(*src, *tgt);
    t.traverse();
    
    t.undoTempInsert(*src);
    t.traverse();
    
    state2->store(t);
   
    ttestr.checkTree(t);
    
    if (!(*state2 == *state1)) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    return failn;
}
