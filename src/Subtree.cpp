//
//  Subtree.cpp
//  Phylogenetic Class Library
//
//  Created by mbrazeau on 29/06/2017.
//  Copyright © 2017 Brazeau Lab. All rights reserved.
//

#include "Subtree.hpp"

void Subtree::init(Node &n)
{
    //reset();
    _start = &n;
    _isrooted = true;
    this->traverse();
}