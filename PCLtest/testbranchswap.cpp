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
    
    int i = 0;
    for (i = 0; i < breaksites.size(); ++i)
    {
        std::cout << "\n\n*** Clipping at " << i << "\n";
        subtr.init(*breaksites.at(i));
        if (i < breaksites.size()-1)
        {
            subtr.clip();
        }
        
        // Enumerate the rerooting sites:
        std::vector<Node*> rootsites;
        
        subtr.doRerootList(rootsites);
        // Store the original rooting site
        Node* orig_site = NULL;
        Node* left = subtr.rootNode()->left();
        Node* right = subtr.rootNode()->right();
        
        int maxroot = (int)rootsites.size();
        
        t.doReconnectList(reconnectsites);
        
        int maxiter = maxroot;
        if (maxroot > 0) {
            ++maxiter;
        }
        
        if (maxroot < 0)
        {
            std::cout << "Break!\n";
        }
        
        int k = 0;
        do
        {
            if (maxroot > 0 && k > 0) { // Only reroot the tree if a) ya can, b) the unrooted tree has already been done
                subtr.root(*rootsites.at(k-1));
            }
            
            int j = 0;
            int max;
            if (i < breaksites.size()-1)
            {
                max = (int)reconnectsites.size();
                assert(max > 0);
            }
            else {
                max = 1;
            }
        
            
            for (j = 0; j < max; ++j)
            {
                std::cout << "--- Reconnecting at " << j << "\n";
                if (i < breaksites.size()-1)
                {
                    t.tempInsert(*subtr.rootNode(), *reconnectsites.at(j));
                }
                t.traverse();
                Topology *saver = new Topology(numtaxa);
                saver->store(t);
                savedtrees.save(*saver);
                ++swapcounter;
                if (i < breaksites.size()-1)
                {
                    t.undoTempInsert(*subtr.rootNode());
                }
            }
            ++k;
        }
        while (k < maxiter);
        
        orig_site = NULL;
        if (left != NULL && right != NULL) {
            if (left->parent() == right)
            {
                orig_site = left;
//                ppass;
            }
            else if (right->parent() == left)
            {
                orig_site = right;
//                ppass;
            }
            else {
                std::cout << "Looks like you're back where you started\n";
//                ++failn;
//                pfail;
            }
        }
        
        if (orig_site != NULL)
        subtr.root(*orig_site);
        
        if (i < breaksites.size()-1)
        {
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
    std::cout << swapcounter << " rearrangements tried\n";
    
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
