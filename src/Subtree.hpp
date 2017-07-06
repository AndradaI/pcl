//
//  Subtree.hpp
//  Phylogenetic Class Library
//
//  Created by mbrazeau on 29/06/2017.
//  Copyright © 2017 Brazeau Lab. All rights reserved.
//

#ifndef Subtree_hpp
#define Subtree_hpp

#include "Tree.hpp"


class Subtree : public Tree
{
    
	Node* oldparent;
	Node* oldchild;

public:
    
    Subtree(int numtaxa)
    : Tree(numtaxa)
    {
        _postorder.reserve(2 * numtaxa - 2);
        _tips.reserve(numtaxa);
        _internals.reserve(numtaxa-1);
    }
    
    Subtree(Node& n, int numtaxa)
    {
        _postorder.reserve(2 * numtaxa - 2);
        init(n);
    }
    
    void init(Node& n);
	void reconnect(Tree& t);
};

#endif /* Subtree_hpp */
