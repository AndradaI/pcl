//
//  BinNode.hpp
//  Phylogenetic Class Library
//
//  Created by mbrazeau on 28/06/2017.
//  Copyright © 2017 Brazeau Lab. All rights reserved.
//

#ifndef BinNode_hpp
#define BinNode_hpp


class BinNode : public Node {
    
    Node* _left;
    Node* _right;
    
public:
    
    BinNode(int index, int tip) : Node(index, tip) { }
    
    Node*   left            (void);
    Node*   right           (void);
    void    addDescendant   (Node& desc);
    
    
protected:
    void rotate(void);
    
};

#endif /* BinNode_hpp */
