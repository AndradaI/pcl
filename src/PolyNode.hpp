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
    
    friend class BinNode;
    
public:
    
    ~PolyNode( ) { }
    
    void    disconnectAll   (void);
    void    addDescendant   (Node& desc);
    
protected:
    void traverse(std::vector<int> inorder);
};

#endif /* PolyNode_hpp */
