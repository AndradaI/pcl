//
//  testTopology.cpp
//  Phylogenetic Class Library
//
//  Created by mbrazeau on 30/06/2017.
//  Copyright © 2017 Brazeau Lab. All rights reserved.
//

#include "pcl.h"
#include "tests.hpp"
#include "TreeTester.hpp"

int test_topology_construction(void)
{
    theader("Testing construction of tree from a topology record");
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
    theader("Testing marking and storage of trees");
    int err     = 0;
    int failn   = 0;
    
    std::string testnwk1 = "((2,((5,6),4)),(1,3));";
    std::string testnwk2 = "((1,3),(2,(4,(5,6))));";
    std::string testnwk3 = "((4,((5,6),2)),(1,3));";
    std::string testnwk4 = "((3,((1,6),4)),(5,2));";
    int numtaxa = 6;
    
    NewickReader reader(numtaxa);
    
    std::cout<< "Storing topology 1\n";
    reader.read(testnwk1, false, true);
    Topology &topol = reader.getTopol();
    
    Tree t(numtaxa);
    
    t.restore(topol);
    
    TreeTester ttestr;
    ttestr.checkTree(t);
    
    std::cout<< "Storing topology 2\n";
    Topology* topol2;
    reader.read(testnwk2, false, true);
    topol2 = &reader.getTopol();
    t.restore(*topol2);
    
    std::cout<< "Storing topology 3\n";
    Topology* topol3;
    reader.read(testnwk3, false, true);
    topol3 = &reader.getTopol();
    t.restore(*topol3);
    
    std::cout<< "Storing topology 4\n";
    Topology* topol4;
    reader.read(testnwk4, false, true);
    topol4 = &reader.getTopol();
    t.restore(*topol4);
    
    if (topol == *topol2)
    {
        std::cout << "Topols 1 & 2 are same\n";
        ppass;
    }
    else {
        std::cout << "Topols 1 & 2 are different\n";
        pfail;
        ++failn;
    }
    
    if (topol == *topol3)
    {
        std::cout << "Topols 1 & 3 are same\n";
        pfail;
        ++failn;
    }
    else {
        std::cout << "Topols 1 & 3 are different\n";
        ppass;
    }
    
    return failn;
}

int test_storing_and_restoring_unrooted_trees(void)
{
    theader("Testing storing and restoring of unrooted trees");
    int err     = 0;
    int failn   = 0;
    
    int numtaxa = 6;
    std::string testnwk1 = "((2,((5,6),4)),(1,3));";
    std::string testnwk2 = "((3,((1,6),4)),(5,2));";
    
    NewickReader reader(numtaxa);
    
    std::cout<< "Storing topology 1\n";
    reader.read(testnwk1, false, true);
    Topology &topol1 = reader.getTopol();
    
    Tree t(numtaxa);
    
    std::cout << "Restoring topology 1 from record\n";
    t.restore(topol1);
    
    TreeTester ttestr;
    ttestr.checkTree(t);
    
    std::cout << "Unrooting the topology\n";
    t.unroot();
    
    ttestr.checkTree(t);
    
    Topology topol2(numtaxa);
    
    std::cout << "Storing the now unrooted tree\n";
    topol2.store(t);
    
    std::cout << "Reading in a new Newick tree and storing it.\n";
    reader.read(testnwk2, false, true);
    Topology& topol3 = reader.getTopol();
    
    t.restore(topol3);
    t.traverse();
    
    std::cout << "Restoring the first tree after unrooting\n";
    t.restore(topol2);
    t.traverse();
    ttestr.checkTree(t);
    t.root();
    t.traverse();
    
    ttestr.checkTree(t);
    return failn;
}

int test_simulated_stepwise_addition(void)
{
    theader("Testing simulated stepwise addition");
    int err     = 0;
    int failn   = 0;
    
    int ntax = 6;
    int hold = 2;
    Tree t(ntax);
    TreeTester ttester;
    
    t.prepStepwise(2, 1, 0);
    t.traverse();
    ttester.checkTree(t);
    
    Node* newtip = t.newTip(3);
    
    Treelist saved(ntax, 0, 0);
    Treelist held(ntax, 0, 0);
    Treelist freetrees(ntax, 2 * hold, 0);
    
    Topology *savetopo;
    Topology *resttopo;
    
    savetopo = freetrees.getTopol();
    savetopo->store(t);
    saved.save(*savetopo);
    
    resttopo = saved.getTopol();
    t.restore(*resttopo);
    
    t.tempInsert(*newtip, *t.postorder(0));
    
    savetopo->store(t);
    saved.save(*savetopo);
    
    t.undoTempInsert(*newtip);
    
    t.traverse();
    
    resttopo = saved.getTopol();
    
    t.restore(*resttopo);
    
    
    
    return failn;
}
