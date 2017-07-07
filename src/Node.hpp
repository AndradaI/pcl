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
    std::vector<Node*>  _descs;
    std::vector<Node*>  _storeddescs;
    
    friend class Tree;
    friend class Subtree;
#ifdef DEBUG
    friend class TreeTester;
#endif
    
public:
    
    Node(int index, int tip)
    :   _index(0),
        _weight(0),
        _in_path(false),
        _downpass_mark(0),
        _is_polynode(false),
        _length(0.0),
        _anc(NULL),
        _left(NULL),
        _right(NULL)
    {
        _mem_index  = index;
        _tip        = tip;
        _descs.reserve(2);
        _storeddescs.reserve(2);
    }
    
    Node(int index, int tip, std::string label) :
    //Node(index, tip)
    _index(0),
    _weight(0),
    _in_path(false),
    _downpass_mark(0),
    _is_polynode(false),
    _length(0.0),
    _anc(NULL),
    _left(NULL),
    _right(NULL)
    {
        _label = label;
    }
    
    virtual ~Node ( )
    {
        
    }
    
    Node*   parent          (void);
    void    parent          (Node& newparent);
    int     parentIndex     (void);
    bool    isInPath        (void);
    int     memIndex        (void);
    int     tipNumber       (void);
    int     uniqueIndex     (void);
    Node*   left            (void);
    int     leftIndex       (void);
    Node*   right           (void);
    int     rightIndex      (void);
    void    disconnectAll   (void);
    void    addDescendant   (Node& desc);
    int     traverse        (std::vector<Node*> &inorder,
                             std::vector<Node*> &tips,
                             std::vector<Node*> &internals);
    int     binTraverse     (std::vector<Node*> &inorder,
                             std::vector<Node*> &tips,
                             std::vector<Node*> &internals);
    void    removeWithBase  (void);
    void    storeDescs      (void);
    void    restoreDescs    (void);
    void    clearStoredDescs(void);
    
protected:
    
    void    rotate          (void);
    void    markTraverse    (int index, bool* found, Node** n);
    void    clip            (void);
    void    restore         (void);
    
private:
    
    void    popDesc         (Node& desc); /*!< Removes desc from node */
    
};

#endif /* Node_hpp */
