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
#include <list>
#include <string>
#include <algorithm>

class Node {
    
    int                 _index;
    int                 _mem_index;
    int                 _tip;
    int                 _weight;
    bool                _in_path;
    int                 _downpass_mark;
    bool                _is_polynode;
    std::string         _label;
    double              _length;
    Node*               _anc;
    Node*               _left;
    Node*               _right;
    std::list<Node*>    _descs;
    
    friend class BinNode;
    friend class PolyNode;
    friend class Tree;
    
public:
    
    Node(int index, int tip)
    :   _weight(0),
        _in_path(false),
        _downpass_mark(0),
        _is_polynode(false),
        _length(0.0),
        _left(NULL),
        _right(NULL),
        _anc(NULL)
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
    int     tipNumber(void);
    Node*   left(void);
    Node*   right(void);
    void    disconnectAll(void);
    void    addDescendant(Node& desc);
    void    traverse(std::vector<Node*> &inorder,
                     std::vector<Node*> &tips,
                     std::vector<Node*> &internals);
    void    binTraverse(std::vector<Node*> &inorder,
                     std::vector<Node*> &tips,
                     std::vector<Node*> &internals);
    void    pop(void);
protected:
    
    void    rotate(void);
    void    markTraverse(int index, bool* found);
    void    clip(void);
    void    restore(void);
    
};

#endif /* Node_hpp */
