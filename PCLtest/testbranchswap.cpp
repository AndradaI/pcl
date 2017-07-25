//
//  testbranchswap.cpp
//  Phylogenetic Class Library
//
//  Created by mbrazeau on 19/07/2017.
//  Copyright © 2017 Brazeau Lab. All rights reserved.
//

#include "pcl.h"
#include "tests.hpp"
#include "TreeTester.hpp"


int test_simple_tbr(void)
{
    theader("Testing a simple TBR routine");
    int err     = 0;
    int failn   = 0;
    
    // Prune the tree.
    // Enumerate all rerootings.
    // Perform all rerootings.
    // Restore original root.
    // Compare with topology from before pruning and rerooting
    
    bool doTBR = false;
    
    unsigned long expected = 0;
    std::string testnwk = "(1,((2,(6,10)),(((3,((4,7),9)),5),8)));"; expected = 322;
    //testnwk = "(((1,5),(3,9)),((((2,4),6),(7,8)),10));"; expected = 270;
    testnwk = "(((((((1,7),8),3),2),4),9),((5,6),10));"; expected = 322;
    if (doTBR == false) expected = 182;
    int numtaxa = 10;
    
    NewickReader reader(numtaxa);
    reader.read(testnwk, false, true);
    Topology &topol = reader.getTopol();
    Treelist savedtrees(numtaxa, 0, 1000);
    
    Tree t(numtaxa);
    
    t.restore(topol);
    t.unroot();
    Subtree subtr(numtaxa);
    
    Topology before(numtaxa);
    before.store(t);
    
    // Store original root:
    std::vector<Node*> breaksites;
    std::vector<Node*> reconnectsites;
    
    t.doBreakList(breaksites);
    
    int swapcounter = 0;
    
    std::vector<Node*> rootsites;
    rootsites.reserve(t.size());    // Reserve a bit of space to minimize reallocs.
    
    int i = 0;
    for (i = 0; i < breaksites.size(); ++i)
    {
        // Initialise a subtree based on the proposed breakpoint.
        subtr.init(*breaksites.at(i));
        
        // Generate a list of possible subtree rerootings
        subtr.doRerootList(rootsites);
        
        // For each possible rerooting, perform the
        int j = 0;
        
//        if (rootsites.size() > 0)
//        {
        // Store original root location
        Node* left = subtr.rootNode()->left();
        Node* right = subtr.rootNode()->right();
        
        for (j = 0; j <= rootsites.size(); ++j )
        {
            // Perform re-root
            if (rootsites.size() > 0 && j > 0)
            {
                subtr.root(*rootsites.at(j-1));
                Topology *topol = new Topology(numtaxa);
                topol->store(t);
                savedtrees.save(*topol);
                ++swapcounter;
            }
            
            if (i < breaksites.size()-1) {
                subtr.clip();
                
                if (doTBR == true)
                {
                    if (j > 0)
                    {
                        t.doTBReconnectList(reconnectsites);
                    }
                    else
                    {
                        t.doReconnectList(reconnectsites);
                    }
                }
                else
                {
                    if (j < 1)
                    {
                        t.doReconnectList(reconnectsites);
                    }
                    else
                    {
                         reconnectsites.clear();
                    }
                }
                
                int k = 0;
                for (k = 0; k < reconnectsites.size(); ++k)
                {
                    t.tempInsert(*subtr.rootNode(), *reconnectsites.at(k));
                    Topology *topol = new Topology(numtaxa);
                    topol->store(t);
                    savedtrees.save(*topol);
                    ++swapcounter;
                    t.undoTempInsert(*subtr.rootNode());
                }
                
                subtr.reconnect();
            }
            
            // Restore the root
            if (rootsites.size() > 0) {
                Node* orig = NULL;
                if (left->parent() == right) {
                    orig = left;
                } else {
                    orig = right;
                }
                subtr.root(*orig);
            }
            
        }
    }
    
    t.traverse();
    Topology after(numtaxa);
    after.store(t);
    
    Topology* restore = NULL;
    i = 1;
    while (savedtrees.numTrees() != 0)
    {
        restore = savedtrees.getTopol();
        t.restore(*restore);
        //std::cout << "tree PAWMtree_" << i << " = " << t.writeNewick();
        ++i;
    }
    //std::cout << "\n\n" << swapcounter << " rearrangements tried\n\n";
    
    if (swapcounter != expected) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    if (before == after) {
        ppass;
    }
    else {
        ++failn;
        pfail;
    }
    
    return failn;
}

int test_big_tbr(void)
{
    theader("Testing SPR/TBR routine on larger tree");
    int err     = 0;
    int failn   = 0;
    
    // Prune the tree.
    // Enumerate all rerootings.
    // Perform all rerootings.
    // Restore original root.
    // Compare with topology from before pruning and rerooting
    
    bool doTBR = true;
    
    unsigned long expected = 0;
    std::string testnwk = "(1,(7,((25,(16,((2,6),(15,(17,(14,24)))))),((21,(8,10)),(9,((11,(12,(19,(4,22)))),(20,(27,(28,(26,(5,(23,(18,(3,13))))))))))))));"; expected = 8250;
    testnwk = "(1,(7,((25,(16,((2,6),(15,(17,(14,24)))))),((21,(8,10)),(9,((20,(27,(28,((5,26),((13,23),(3,18)))))),(11,(12,(19,(4,22))))))))));"; expected = 7906;
    testnwk = "(1,(7,(((21,(8,10)),(9,((20,(27,(28,((5,26),((13,23),(3,18)))))),(11,(12,(19,(4,22))))))),((25,(16,(2,6))),(15,(17,(14,24)))))));"; expected = 7474;
    testnwk = "(1,(7,((25,(16,((2,6),(15,(17,(14,24)))))),((21,(8,10)),(9,((20,(27,(28,(26,(5,((13,23),(3,18))))))),(11,(12,(19,(4,22))))))))));"; expected = 8158;
    testnwk = "(1,(7,((25,(16,((2,6),(15,(17,(14,24)))))),((21,(8,10)),(9,((20,(27,(28,(5,(26,((13,23),(3,18))))))),(11,(12,(19,(4,22))))))))));"; expected = 8158;
    testnwk = "(1,(7,((25,(16,((2,6),(15,(17,(14,24)))))),((21,(8,10)),(9,((20,(27,(28,((5,26),(13,(23,(3,18))))))),(11,(12,(19,(4,22))))))))));"; expected = 7998;
    //   testnwk = "(1,(7,((25,(16,((2,6),(15,(17,(14,24)))))),((21,(8,10)),(9,((20,(27,(28,(26,(5,(13,(23,(3,18)))))))),(11,(12,(19,(4,22))))))))));"; expected = 8250;
//    testnwk = "(1,(7,((25,(16,((2,6),(15,(17,(14,24)))))),((21,(8,10)),(9,((11,(12,(19,(4,22)))),(20,(27,(28,(26,(5,(23,(18,(3,13))))))))))))));"; expected = 8250;
//    testnwk = "(1,(7,((25,(16,((2,6),(15,(17,(14,24)))))),((21,(8,10)),(9,((11,(12,(19,(4,22)))),(20,(27,(28,(26,(5,(23,(18,(3,13))))))))))))));"; expected = 8250;
//    testnwk = "(1,(7,((25,(16,((2,6),(15,(17,(14,24)))))),((21,(8,10)),(9,((11,(12,(19,(4,22)))),(20,(27,(28,(26,(5,(23,(18,(3,13))))))))))))));"; expected = 8250;
    testnwk = "(1,(((25,((15,(17,(14,24))),(6,(2,16)))),(21,(8,10))),(7,((9,(20,(11,(12,19)))),(((27,28),((5,26),((13,23),(3,18)))),(4,22))))));"; expected = 5035;
    if (doTBR == false) expected = 2450;
    int numtaxa = 28;
    
    NewickReader reader(numtaxa);
    reader.read(testnwk, false, true);
    Topology &topol = reader.getTopol();
    Treelist savedtrees(numtaxa, 0, 1000);
    
    Tree t(numtaxa);
    
    t.restore(topol);
    t.unroot();
    Subtree subtr(numtaxa);
    
    Topology before(numtaxa);
    before.store(t);
    
    // Store original root:
    std::vector<Node*> breaksites;
    std::vector<Node*> reconnectsites;
    
    t.doBreakList(breaksites);
    
    int swapcounter = 0;
    
    subtr.init(*breaksites.at(20));
    subtr.clip();
    subtr.reconnect();
    
    subtr.init(*breaksites.at(21));
    subtr.clip();
    subtr.reconnect();
    
    subtr.init(*breaksites.at(32));
    subtr.clip();
    subtr.reconnect();
    
    std::vector<Node*> rootsites;
    rootsites.reserve(t.size());    // Reserve a bit of space to minimize reallocs.
    
    int i = 0;
    for (i = 0; i < breaksites.size(); ++i)
    {
        // Initialise a subtree based on the proposed breakpoint.
        subtr.init(*breaksites.at(i));
        
        // Generate a list of possible subtree rerootings
        subtr.doRerootList(rootsites);
        
        // For each possible rerooting, perform the
        int j = 0;
        
//        if (rootsites.size() > 0)
//        {
        // Store original root location
        Node* left = subtr.rootNode()->left();
        Node* right = subtr.rootNode()->right();
        
        for (j = 0; j <= rootsites.size(); ++j )
        {
            // Perform re-root
            if (rootsites.size() > 0 && j > 0)
            {
                subtr.root(*rootsites.at(j-1));
                t.root();
                t.simpleUnroot();
                Topology *topol = new Topology(numtaxa);
                topol->store(t);
                savedtrees.save(*topol);
                ++swapcounter;
            }
            
            if (i < breaksites.size()-1) {
                subtr.clip();
                
                if (doTBR == true)
                {
                    if (j > 0)
                    {
                        t.doTBReconnectList(reconnectsites);
                    }
                    else
                    {
                        t.doReconnectList(reconnectsites);
                    }
                }
                else
                {
                    if (j < 1)
                    {
                        t.doReconnectList(reconnectsites);
                    }
                    else
                    {
                         reconnectsites.clear();
                    }
                }
                
                int k = 0;
                for (k = 0; k < reconnectsites.size(); ++k)
                {
                    t.tempInsert(*subtr.rootNode(), *reconnectsites.at(k));
                    t.root();
                    t.simpleUnroot();
                    Topology *topol = new Topology(numtaxa);
                    topol->store(t);
                    savedtrees.save(*topol);
                    ++swapcounter;
                    t.undoTempInsert(*subtr.rootNode());
                }
                
                subtr.reconnect();
            }
            
            // Restore the root
            if (rootsites.size() > 0) {
                Node* orig = NULL;
                if (left->parent() == right) {
                    orig = left;
                } else {
                    orig = right;
                }
                subtr.root(*orig);
            }
            
        }
    }
    
    t.traverse();
    Topology after(numtaxa);
    after.store(t);
    
    
    Topology* restore = NULL;
    i = 1;
    while (savedtrees.numTrees() != 0)
    {
        restore = savedtrees.getTopol();
        t.restore(*restore);
        //std::cout << "tree PAWMtree_" << i << " = " << t.writeNewick();
        ++i;
    }
    //std::cout << "\n\n" << swapcounter << " rearrangements tried\n\n";
    
    if (swapcounter != expected) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    if (before == after) {
        ppass;
    }
    else {
        ++failn;
        pfail;
    }
    
    return failn;
}
