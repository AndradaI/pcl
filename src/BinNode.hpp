//
//  BinNode.hpp
//  Phylogenetic Class Library
//
//  Created by mbrazeau on 28/06/2017.
//  Copyright © 2017 Brazeau Lab. All rights reserved.
//

#ifndef BinNode_hpp
#define BinNode_hpp

#include <vector>


class BinNode : public Node {
    
    Node* _left;
    Node* _right;
    
    friend class PolyNode;
    
public:
    
    BinNode(int index, int tip) : Node(index, tip) { }
    
    ~BinNode() { }
    
    Node*   left            (void);
    Node*   right           (void);
    void    makeBinary      (void);
    void    disconnectAll   (void);
    void    addDescendant   (Node& desc);
    
    
protected:
    
    void rotate(void);
    void traverse(std::vector<Node*> &inorder,
                  std::vector<Node*> &tips,
                  std::vector<Node*> &internals);
    
};

#endif /* BinNode_hpp */
