//
//  testTreelist.cpp
//  Phylogenetic Class Library
//
//  Created by mbrazeau on 18/07/2017.
//  Copyright © 2017 Brazeau Lab. All rights reserved.
//

#include "tests.hpp"
#include "pcl.h"

int test_duplicate_checks(void)
{
    theader("Testing checks for duplicate trees");
    int err     = 0;
    int failn   = 0;

    int ntax = 10;
    int ntrees = 12;
    
    // 12 random 10-taxon trees generated by PAUP*. 
    std::string newick1 = "((((1,((2,7),(5,9))),(4,8)),6),(3,10));";
    std::string newick2 = "((1,3),(((((((2,10),6),8),9),7),5),4));";
    std::string newick3 = "(((1,(((2,4),3),((7,9),8))),(5,10)),6);";
    std::string newick4 = "(((((((1,8),(3,4)),(2,6)),9),7),5),10);";
    std::string newick5 = "((((((1,(4,6)),10),5),9),((2,8),7)),3);";
    std::string newick6 = "((((8,4),(((5,9),(2,7)),1)),6),(10,3));"; // Same as 1
//    std::string newick7 = "((((((((1,2),3),(8,10)),9),7),4),6),5);";
//    std::string newick8 = "(((((1,5),(((2,10),3),4)),9),6),(7,8));";
//    std::string newick9 = "((((((1,6),(3,(4,8))),7),10),(2,5)),9);";
//    std::string newick10 = "(((((1,2),10),(3,(5,((7,8),9)))),4),6);";
//    std::string newick11 = "(1,(((2,(((3,6),((5,10),7)),4)),9),8));";
//    std::string newick12 = "((((((1,10),5),((4,6),7)),2),9),(3,8));";
    
    Treelist freetopols(ntax, ntrees, 0);
    Treelist savetopols(ntax, 0, 0);
    Topology* savet;
    NewickReader reader(ntax);
    
    reader.read(newick1, 0, true);
    savet = &reader.getTopol();
    
    if (!savetopols.checkNew(*savet))
    {
        savetopols.save(*savet);
        ppass;
    }
    else {
        ++failn;
        pfail;
    }
    
    reader.read(newick2, 0, true);
    savet = &reader.getTopol();
    if (!savetopols.checkNew(*savet))
    {
        savetopols.save(*savet);
        ppass;
    }
    else {
        ++failn;
        pfail;
    }
    
    reader.read(newick3, 0, true);
    savet = &reader.getTopol();
    if (!savetopols.checkNew(*savet))
    {
        savetopols.save(*savet);
        ppass;
    }
    else {
        ++failn;
        pfail;
    }
    
    reader.read(newick4, 0, true);
    savet = &reader.getTopol();
    if (!savetopols.checkNew(*savet))
    {
        savetopols.save(*savet);
        ppass;
    }
    else {
        ++failn;
        pfail;
    }
    
    reader.read(newick5, 0, true);
    savet = &reader.getTopol();
    if (!savetopols.checkNew(*savet))
    {
        savetopols.save(*savet);
        ppass;
    }
    else {
        ++failn;
        pfail;
    }
    
    reader.read(newick6, 0, true);
    savet = &reader.getTopol();
    if (savetopols.checkNew(*savet))
    {
        ppass;
    }
    else {
        ++failn;
        pfail;
    }
    
    if (savetopols.checkNew(*savet))
    {
        ppass;
    }
    else {
        ++failn;
        pfail;
    }
    return failn;
}

int test_large_tree_comparison(void)
{
    theader("Testing large tree comparison");
    int err     = 0;
    int failn   = 0;
    
    std::string newick1 = "(1,(7,((25,(16,((15,(17,(14,24))),(2,6)))),((21,(8,10)),(9,((20,(27,(28,(5,(26,(23,(3,(13,18)))))))),(11,(12,(19,(4,22))))))))));";
    std::string newick2 = "(1,(7,((25,(16,((15,(17,(14,24))),(2,6)))),((21,(8,10)),(9,((20,(27,(28,(5,(26,(23,(13,(3,18)))))))),(11,(12,(19,(4,22))))))))));";
    std::string newick3 = "(1,(7,((25,(16,((15,(17,(14,24))),(2,6)))),((21,(8,10)),(9,((20,(27,(28,(5,(23,(26,(13,(3,18)))))))),(11,(12,(19,(4,22))))))))));";
    std::string newick4 = "(1,(7,((25,(16,((15,(17,(14,24))),(2,6)))),((21,(8,10)),(9,((20,(27,(28,(5,((23,26),(13,(3,18))))))),(11,(12,(19,(4,22))))))))));";
    std::string newick5 = "(1,(7,((25,(16,((15,(17,(14,24))),(2,6)))),((21,(8,10)),(9,((20,(27,(28,(5,(23,(26,(3,(13,18)))))))),(11,(12,(19,(4,22))))))))));";
    std::string newick6 = "(1,(7,((25,(16,((15,(17,(14,24))),(2,6)))),((21,(8,10)),(9,((20,(27,(28,(26,(5,(23,(3,(13,18)))))))),(11,(12,(19,(4,22))))))))));";
    std::string newick7 = "(1,(7,((25,(16,((15,(17,(14,24))),(2,6)))),((21,(8,10)),(9,((20,(27,(28,(5,(26,(23,(3,(13,18)))))))),(11,(12,(19,(4,22))))))))));";
    
    int ntax = 28;
    
    Tree t(ntax);
    Treelist trlist(ntax, 0, 10);
    Topology *restore;
    NewickReader reader(ntax);
    
    std::cout << "\n\nDoing the restorations\n\n";
    
    reader.read(newick1, false, false);
    restore = &reader.getTopol();
    t.restore(*restore);
    trlist.save(t);
    reader.read(newick2, false, false);
    restore = &reader.getTopol();
    t.restore(*restore);
    trlist.save(t);
    reader.read(newick3, false, false);
    restore = &reader.getTopol();
    t.restore(*restore);
    trlist.save(t);
    reader.read(newick4, false, false);
    restore = &reader.getTopol();
    t.restore(*restore);
    trlist.save(t);
//    reader.read(newick5, false, false);
//    restore = &reader.getTopol();
//    t.restore(*restore);
//    trlist.save(t);
//    reader.read(newick6, false, false);
//    restore = &reader.getTopol();
//    t.restore(*restore);
//    trlist.save(t);
    reader.read(newick7, false, false);
    restore = &reader.getTopol();
    t.restore(*restore);
    
    std::cout << "\n\nDoing the comparisons:\n\n";
    
    bool dupe = false;
    
    dupe = trlist.checkNew(t);
    
    if (dupe == false) {
        ++failn;
        pfail;
    }
    else {
        ppass;
    }
    
    return failn;
}

