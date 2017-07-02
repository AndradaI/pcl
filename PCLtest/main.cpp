//
//  main.cpp
//  PCLtest
//
//  Created by mbrazeau on 28/06/2017.
//  Copyright © 2017 Brazeau Lab. All rights reserved.
//

#include <iostream>
#include "pcl.h"
#include "tests.hpp"

int main(int argc, const char * argv[]) {
    
    int failn = 0;
    
    failn += test_newick_reader_basic();
    failn += test_topology_construction();
    failn += test_rerooting();
    failn += test_subtrees_and_rerooting();
    failn += test_tree_marking();
    return failn;
}
