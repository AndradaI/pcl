//
//  PolyNode.cpp
//  Phylogenetic Class Library
//
//  Created by mbrazeau on 29/06/2017.
//  Copyright © 2017 Brazeau Lab. All rights reserved.
//

#include "PolyNode.hpp"

void PolyNode::addDescendant(Node &desc)
{
    descs.push_back(desc);
    
    desc.parent(*this);
}
