//
//  testSubtree.cpp
//  Phylogenetic Class Library
//
//  Created by mbrazeau on 18/07/2017.
//  Copyright © 2017 Brazeau Lab. All rights reserved.
//

#include "pcl.h"
#include "tests.hpp"
#include "TreeTester.hpp"

int test_single_taxon_subtree(void)
{
    theader("Testing single-taxon subtrees");
    int err     = 0;
    int failn   = 0;
    
    int numtaxa = 6;
    
    std::string testnwk = "((6,((5,2),4)),(1,3));";
    
    NewickReader reader(numtaxa);
    reader.read(testnwk, false, true);
    Topology &topol = reader.getTopol();
    
    Tree t(numtaxa);
    
    t.restore(topol);
    
    Subtree subtr(numtaxa-2);
    
    Node* tip = NULL;
    tip = t.newTip(5);
    
    subtr.init(*tip);
    
    // TODO: Make safe for attempts to root
    subtr.root(2);
    
    //std::cout << subtr.writeNewick() << std::endl;

    std::string expected = "[&R] 6;";
    
    if (expected != subtr.writeNewick()) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    return failn;
}

int test_subtree_removal_reconnection(void)
{
    theader("Testing subtree removal and reconnection");
    int err     = 0;
    int failn   = 0;
    
    std::string testnwk = "(((1,5),(3,9)),((((2,4),6),(7,8)),10));";
    int numtaxa = 10;
    
    NewickReader reader(numtaxa);
    reader.read(testnwk, false, true);
    Topology &topol = reader.getTopol();
    
    Tree t(numtaxa);
    
    t.restore(topol);
    Subtree subtr(numtaxa);
    Node* node = t.postorderIntern(3);
    subtr.init(*node);
    
    subtr.clip();
    
    //std::cout << "Original tree after the clip:\n";
    t.traverse();
    
    subtr.reconnect();
    
    //std::cout << "Original tree after restoring:\n";
    t.traverse();
    TreeTester ttestr;
    err = ttestr.checkTree(t);
    
    if (err) {
        ++failn;
        pfail;
        err = 0;
    }
    else {
        ppass;
    }
    
    return failn;
}

int test_all_subtree_rerootings(void)
{
    theader("Testing doing all rerootings of subtree");
    int err     = 0;
    int failn   = 0;
    
    // Prune the tree.
    // Enumerate all rerootings.
    // Perform all rerootings.
    // Restore original root.
    // Compare with topology from before pruning and rerooting
    
    std::string testnwk = "(((1,5),(3,9)),((((2,4),6),(7,8)),10));";
    int numtaxa = 10;
    
    NewickReader reader(numtaxa);
    reader.read(testnwk, false, true);
    Topology &topol = reader.getTopol();
    
    Tree t(numtaxa);
    
    t.restore(topol);
    t.unroot();
    t.traverse();
    Subtree subtr(numtaxa);
    Node* node = t.postorderIntern(3);
    subtr.init(*node);
    
    Topology before(numtaxa);
    before.store(t);
    
    subtr.clip();
    
    // Store original root:
    Node* left = subtr.rootNode()->left();
    Node* right = subtr.rootNode()->right();
    // Enumerate the rerooting sites:
    std::vector<Node*> rootsites;
    subtr.traverse(rootsites);
    int maxroot = (int)rootsites.size()-1;
    int i = 0;
    for (i = 0; i < maxroot; ++i)
    {
        subtr.root(*rootsites[i]);
    }
    
    // TODO: Now figure out a way to restore the root.
    Node* orig_site = NULL;
    if (left->parent() == right)
    {
        orig_site = left;
        ppass;
    }
    else if (right->parent() == left)
    {
        orig_site = right;
        ppass;
    }
    else {
        //std::cout << "Looks like you're back where you started\n";
        ++failn;
        pfail;
    }
    
    subtr.root(*orig_site);
    
    subtr.reconnect();
    t.traverse();
    Topology after(numtaxa);
    after.store(t);
    
    if (before == after) {
        ppass;
    }
    else {
        ++failn;
        pfail;
    }
    
    return failn;
}
