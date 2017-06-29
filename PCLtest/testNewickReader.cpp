//
//  testNewickReader.cpp
//  Phylogenetic Class Library
//
//  Created by mbrazeau on 28/06/2017.
//  Copyright © 2017 Brazeau Lab. All rights reserved.
//

#include "NewickReader.hpp"
#include "testNewickReader.hpp"

int test_newick_reader_basic(void)
{
    //theader("Basic test of Newick reader");
    int err     = 0;
    int failn   = 0;
    
    std::string testnwk = "((2,((5,6),4)),(1,3));";
    int numtaxa = 6;
    
    NewickReader nwkreader(numtaxa);
    nwkreader.read(testnwk, false, false);
    
    
    return failn;
}
