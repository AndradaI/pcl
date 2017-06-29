//
//  PolyNode.hpp
//  Phylogenetic Class Library
//
//  Created by mbrazeau on 29/06/2017.
//  Copyright © 2017 Brazeau Lab. All rights reserved.
//

#ifndef PolyNode_hpp
#define PolyNode_hpp

#include <list>

#include "Node.hpp"

class PolyNode : public Node {
    
    std::list<Node*> descs;
    
public:
    
    void addDescendant(Node& desc);
};

#endif /* PolyNode_hpp */
