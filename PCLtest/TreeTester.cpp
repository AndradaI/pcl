//
//  TreeTester.cpp
//  Phylogenetic Class Library
//
//  Created by mbrazeau on 01/07/2017.
//  Copyright © 2017 Brazeau Lab. All rights reserved.
//

#include "TreeTester.hpp"

int TreeTester::checkTree(Tree &t)
{
    int errors(0);
    
    innodes.clear();
    ancestors.clear();
    
    int i = 0;
    int max = static_cast<int>(t._nodes.size());
    
    for (i = 0; i < max; ++i)
    {
        if (t._nodes[i]->parent() != NULL)
        {
            innodes.push_back(t._nodes[i]);
            ancestors.push_back(t._nodes[i]->parent());
        }
    }
    
    std::vector<Node*>::iterator p = ancestors.begin();
    std::vector<Node*>::iterator q = innodes.begin();
    
    i = 0;
    
    for (; p != ancestors.end() && *q != t._dummy_root; ++p, ++q)
    {
        if (!findDesc(*q, *p))
        {
            ++errors;
            std::cout << "Your goddamned tree is broken!\n";
        }
        ++i;
    }
    
    return errors;
}


bool TreeTester::findDesc(Node *tgt, Node *n)
{
    bool found = false;
    
    std::vector<Node*>::iterator i;
    
    for (i = n->_descs.begin(); i != n->_descs.end(); ++i)
    {
        if (*i == tgt) {
            found = true;
            break;
        }
    }
    
    return found;
}
