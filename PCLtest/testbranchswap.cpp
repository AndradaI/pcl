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
    
    Tree t(numtaxa);
    
    t.restore(topol);
    t.unroot();
    Subtree subtr(numtaxa);
    Node* node = t.postorderIntern(3);
    subtr.init(*node);
    
    Topology before(numtaxa);
    before.store(t);
    
    // Store original root:
    
    
    std::vector<Node*> breaksites;
    std::vector<Node*> reconnectsites;
    
    t.doBreakList(breaksites);
    
    int i = 0;
    for (i = 0; i < breaksites.size()-1; ++i)
    {
        std::cout << "\n\n*** Clipping at " << i << "\n";
        subtr.init(*breaksites.at(i));
        subtr.clip();
        
        // Enumerate the rerooting sites:
        std::vector<Node*> rootsites;
        
        subtr.traverse(rootsites);
        // Store the original rooting site
        Node* orig_site = NULL;
        Node* left = subtr.rootNode()->left();
        Node* right = subtr.rootNode()->right();
        
        int maxroot = (int)rootsites.size()-1;
        int k = 0;
        
        do
        {
            if (maxroot > 2) { // Only reroot the tree if a) ya can, b) the unrooted tree has already been done
                subtr.root(*rootsites[k]);
            }
            
            int j = 0;
            t.doReconnectList(reconnectsites);
            
            for (j = 0; j < reconnectsites.size(); ++j)
            {
                if ((reconnectsites.at(j)->isClipSite() == false) || k > 0)
                {
                    std::cout << "--- Reconnecting at " << j << "\n";
                    t.tempInsert(*subtr.rootNode(), *reconnectsites.at(j));
                    t.traverse();
                    t.undoTempInsert(*subtr.rootNode());
                }
                else {
                    std::cout << "Skipping...\n";
                }
            }
            ++k;
        }
        while (k < maxroot-1);
        
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
        
        subtr.root(*orig_site);
        subtr.reconnect();
    }
    
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
