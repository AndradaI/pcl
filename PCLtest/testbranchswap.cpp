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
    
    std::string testnwk = "(1,((2,(6,10)),(((3,((4,7),9)),5),8)));";
    //    std::string testnwk = "(((1,5),(3,9)),((((2,4),6),(7,8)),10));";
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
        rootsites.clear();
        subtr.doRerootList(rootsites);
        int max = rootsites.size();
        
        // Store original root location
        Node* left = subtr.rootNode()->left();
        Node* right = subtr.rootNode()->right();
        
        // For each possible rerooting, perform the
        int j = 0;
        
        if (rootsites.size() > 0)
        {
            for (j = 0; j < max; ++j )
            {
                // Perform re-root
                subtr.root(*rootsites.at(j));
                Topology *topol = new Topology(numtaxa);
                topol->store(t);
                savedtrees.save(*topol);
                ++swapcounter;
                
                if (i < breaksites.size()-1 && j > 2) {
                    subtr.clip();
                    
                    t.doReconnectList(reconnectsites);

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
                Node* orig = NULL;
                if (left->parent() == right) {
                    orig = left;
                } else {
                    orig = right;
                }
                subtr.root(*orig);
                
            }
        }
        else
        {
            subtr.clip();
            
            t.doReconnectList(reconnectsites);
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
        std::cout << "tree PAWMtree_" << i << " = " << t.writeNewick();
        ++i;
    }
    std::cout << "\n\n" << swapcounter << " rearrangements tried\n\n";
    
    if (swapcounter != 322) {
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
