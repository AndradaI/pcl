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
    
    int fails = 0;
    
    fails += test_newick_reader_basic();
    fails += test_topology_construction();
    fails += test_rerooting();
    fails += test_unrooted_trees();
    fails += test_subtrees_and_rerooting();
    fails += test_tree_marking();
    fails += test_node_pop();
    fails += test_stepwise_addition();
    fails += test_stepwise_addition_larger();
    fails += test_temp_insert_and_restore();
    fails += test_storing_and_restoring_unrooted_trees();
    fails += test_simulated_stepwise_addition();
    fails += test_duplicate_checks();
    fails += test_write_newick();
    fails += test_single_taxon_subtree();
    fails += test_subtree_removal_reconnection();
    fails += test_all_subtree_rerootings();
    fails += test_simple_tbr();
    fails += test_large_tree_comparison();
    fails += test_big_tbr();
    
    if (fails) {
        psumf(fails);
    }
    else {
        psump;
    }
    
    return fails;
}
