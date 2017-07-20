//
//  Subtree.hpp
//  Phylogenetic Class Library
//
//  Created by mbrazeau on 29/06/2017.
//  Copyright © 2017 Brazeau Lab. All rights reserved.
//

#ifndef Subtree_hpp
#define Subtree_hpp

#include <assert.h>
#include "Tree.hpp"


class Subtree : public Tree
{
    
	Node*               _oldparent;
    std::vector<Node*>  _oldp_descs;
	Node*               _oldchild;
    std::vector<Node*>  _oldc_descs;
    Node*               _subtr_root;

public:
    
    Subtree(int numtaxa)
    :   Tree(numtaxa),
        _subtr_root(NULL)
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
    void clip(void);
	void reconnect(void);
    void root(int index);
    void root(Node& n);
    Node* rootNode(void);
};

#endif /* Subtree_hpp */
