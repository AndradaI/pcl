//
//  TreeTester.hpp
//  Phylogenetic Class Library
//
//  Created by mbrazeau on 01/07/2017.
//  Copyright © 2017 Brazeau Lab. All rights reserved.
//

#ifndef TreeTester_hpp
#define TreeTester_hpp

#include <iostream>
#include <vector>
#include "Tree.hpp"
#include "Node.hpp"

class TreeTester {
    
    std::vector<Node*> innodes;
    std::vector<Node*> ancestors;
    
public:
    
    int checkTree(Tree& t);
    
private:
    
    bool findDesc(Node* tgt, Node* n);
};

#endif /* TreeTester_hpp */
