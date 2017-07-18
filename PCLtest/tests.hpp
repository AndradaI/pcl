//
//  testNewickReader.hpp
//  Phylogenetic Class Library
//
//  Created by mbrazeau on 28/06/2017.
//  Copyright © 2017 Brazeau Lab. All rights reserved.
//

#ifndef testNewickReader_hpp
#define testNewickReader_hpp

#include <stdio.h>
#include <iostream>
#include <vector>

#define pfail \
printf("[  FAIL  ] %s, line: %i in: %s\n", __FUNCTION__, __LINE__, __FILE__)

#define ppass printf("[  PASS  ] %s\n", __FUNCTION__)

#define psumf(...) \
printf("[  ** FAILED ** ] %i times. Review output to see details.\n", fails)

#define psump printf("[  ** PASSED ** ] All tests passed.\n")

#define theader(testname) printf("\n\n\t%s\n\n", testname);

int test_newick_reader_basic(void);
int test_topology_construction(void);
int test_tree_marking(void);
int test_rerooting(void);
int test_unrooted_trees(void);
int test_subtrees_and_rerooting(void);
int test_stepwise_addition(void);
int test_stepwise_addition_larger(void);
int test_node_pop(void);
int test_temp_insert_and_restore(void);
int test_storing_and_restoring_unrooted_trees(void);
int test_simulated_stepwise_addition(void);
int test_duplicate_checks(void);
int test_write_newick(void);
int test_single_taxon_subtree(void);
int test_subtree_removal_reconnection(void);
int test_all_subtree_rerootings(void);
#endif /* testNewickReader_hpp */
