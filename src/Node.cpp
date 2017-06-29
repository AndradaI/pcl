//
//  Node.cpp
//  Phylogenetic Class Library
//
//  Created by mbrazeau on 28/06/2017.
//  Copyright © 2017 Brazeau Lab. All rights reserved.
//

#include "Node.hpp"

Node* Node::parent(void)
{
    if (_anc != NULL) {
        return _anc;
    }
    
    return NULL;
}

void Node::parent(Node& newparent)
{
    _anc = &newparent;
}

bool Node::isInPath(void)
{
    return _in_path;
}
