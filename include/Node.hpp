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
#include <assert.h>

class Node {
    
    int                 _index;     /*!< Unique in-tree identifier based on tip-ordered downpass */
    int                 _mem_index; /*!< Unique serial identifier. Do not change. */
    int                 _tip;       /*!< A non-zero identifier if the node is a tip. 0 if not a tip. */
    int                 _weight;    /*!< The number of terminal descendants from this node. */
    bool                _in_path;   /*!< For marking paths to nodes. Used in updates and rerooting. */
    bool                _is_outgroup_member;
    bool                _is_clipsite; /*!< Marks the location of a clipping in the root part of the tree. */
    int                 _downpass_mark; /*!< I don't remember. */
    bool                _tempmark;
    std::string         _label;         /*!< The name of this node (if applied). */
    double              _length; /*!< The length of the branch subtending this node. */
    Node*               _anc;   /*!< Pointer to the immediate ancestor fo this node. */
    Node*               _left;  /*!< Pointer to the left descendant of this node. */
    Node*               _right; /*!< Pointer to the right descendant of this node. */
    std::vector<Node*>  _descs; /*!< Pointers to all descendants of this node. */
    std::vector<Node*>  _storeddescs; /*!< Storage for current descendant values for temporary topology changes. */
    
    friend class Tree; /*!< Trees have access to private members of nodes. */
    friend class Subtree; /*!< Subtrees have access to private members of nodes. */
    friend class Topology;
#ifdef DEBUG
    friend class TreeTester; /*!< In debug mode, TreeTesters have access to private members of nodes. */
#endif
    
public:
    
    Node(int index = 0, int tip = 0)
    :   _index(0),
        _weight(0),
        _in_path(false),
        _is_outgroup_member(false),
        _downpass_mark(0),
        _tempmark(false),
    //_is_polynode(false),
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
    _tempmark(false),
    //_is_polynode(false),
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
    Node*   sibling         (void);
    void    assignIndex     (unsigned long index);
    int     parentIndex     (void);
    bool    isPolynode      (void);
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
    void    writeNewick     (std::string &Newick);
    void    removeWithBase  (void);
    void    storeDescs      (void);
    void    restoreDescs    (void);
    void    restoreDescs    (std::vector<Node*> &descs);
    void    clearStoredDescs(void);
    void    clearDescs      (void);
    void    outgroup        (void);
    void    ingroup         (void);
    bool    isOutgroup      (void);
    void    markClipSite    (void);
    void    unmarkClipSite  (void);
    bool    isClipSite      (void);
    void    travBreakList   (std::vector<Node*> &breaksites, const int max_subtr_size);
    void    travReconnectList(std::vector<Node*> &reconnectsites);
    void    travTBReconnectList(std::vector<Node *> &reconnectsites);
    void    tempmark        (void);
    void    undoTempmark    (void);
    bool    isTempmarked    (void);
    
protected:
    
    void    rotate          (void);
    void    markTraverse    (int index, bool* found, Node** n);
    void    clip            (void);
    void    restore         (void);
    
private:
    
    void    popDesc         (Node& desc); /*!< Removes desc from node */
    
};

#endif /* Node_hpp */
