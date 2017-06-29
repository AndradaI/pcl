//
//  Node.hpp
//  Phylogenetic Class Library
//
//  Created by mbrazeau on 28/06/2017.
//  Copyright © 2017 Brazeau Lab. All rights reserved.
//

#ifndef Node_hpp
#define Node_hpp

#include <vector>
#include <string>

class Node {
    
    int         _index;
    int         _mem_index;
    int         _tip;
    int         _weight;
    bool        _in_path;
    int         _downpass_mark;
    bool        _is_polynode;
    std::string _label;
    double      _length;
    Node*       _anc;
    
    friend class BinNode;
    friend class PolyNode;
    friend class Tree;
    
public:
    
    Node(int index, int tip)
    {
        _mem_index  = index;
        _tip        = tip;
    }
    
    Node(int index, int tip, std::string label) :
    Node(index, tip)
    {
        _label = label;
    }
    
    virtual ~Node ( )
    {
        
    }
    
    Node*   parent(void);
    void    parent(Node& newparent);
    bool    isInPath(void);
    int     memIndex(void);
    
    virtual void disconnectAll(void) = 0;
    virtual void addDescendant(Node& desc) = 0;
    
protected:
    
    void            clip(void);
    void            restore(void);
    virtual void    traverse(std::vector<Node*> inorder,
                             std::vector<Node*> tips,
                             std::vector<Node*> internals) = 0;
    
};

#endif /* Node_hpp */
