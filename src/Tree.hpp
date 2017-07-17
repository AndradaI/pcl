//
//  Tree.hpp
//  Phylogenetic Class Library
//
//  Created by mbrazeau on 28/06/2017.
//  Copyright © 2017 Brazeau Lab. All rights reserved.
//

#ifndef Tree_hpp
#define Tree_hpp

#include <vector>
#include <iostream>
#include <algorithm>

#include "Node.hpp"

class Topology;
class Subtree;

class Tree
{
    
    //protected:
    
    int                             _num_taxa;
    bool                            _isrooted;
    Node*                           _start;
    double                          _real_score;
    Subtree*                        _subtree;
    unsigned long                   _natural_score;
    std::vector<Node*>              _postorder;
    
    //private:
    
    Node*                           _reserved_root;
    Node*                           _dummy_root;
    int                             _starttip;
    std::vector<Node*>              _nodes;
    std::vector<Node*>              _tips;
    std::vector<Node*>              _internals;
    std::vector<Node*>              _free_tips;
    std::vector<Node*>              _free_vertices;
    //std::vector<Node*>::iterator    _nextFreeTip;
    //std::vector<Node*>::iterator    _nextFreeInternal;
    std::vector<Node*>::iterator    _nextFreeOutgroup;
    
public:
    
    friend class Topology;
    friend class Subtree;
#ifdef DEBUG
    friend class TreeTester;
#endif
    
    Tree ()
    {
        
    }
    
    Tree(int numtaxa)
    :   _start(NULL),
        _real_score(0.0),
        _subtree(NULL),
        _natural_score(0),
        _starttip(0)
    {
        int num_nodes = 2 * numtaxa;
        _num_taxa = numtaxa;
        
        _nodes.reserve(num_nodes);
        
        Node* newnode = NULL;
        int index = 0;
        
        for (index = 0; index < num_nodes; ++index)
        {
            newnode = NULL;
            
            if (index < numtaxa)
            {
                newnode = new Node(index, index + 1);
                _free_tips.push_back(newnode);
            }
            else
            {
                if (index < num_nodes - 1)
                {
                    newnode = new Node(index, 0);
                    
                    if (index == num_nodes - 2)
                    {
                        _reserved_root = newnode;
                    }
                    else {
                        _free_vertices.push_back(newnode);
                    }
                }
                else {
                    newnode = new Node(index, -1);
                    _dummy_root = newnode;
                }
            }
            _nodes.push_back(newnode);
        }
        
        reset();
    }
    
    ~Tree()
    {
        std::vector<Node*>::iterator it;
        
        for (it = _nodes.begin(); it != _nodes.end(); ++it)
        {
            delete *it;
        }
    }
    
    bool            isrooted        (void);
    unsigned long   size            (void);
    unsigned long   capacity        (void);
    unsigned long   numVertices     (void);
    int             numTaxa         (void);
    int             numTips         (void);
    void            restore         (Topology& topol);
    void            reset           (void);
    void            incrScore       (unsigned long s);
    void            incrScore       (double s);
    void            setScore        (double s);
    void            setScore        (unsigned long s);
    void            clearScore      (void);
    unsigned long   natScore        (void);
    double          realScore       (void);
    Node*           newTip          (int index);
    Node*           newVertex       (void);
    Node*           node            (int index);
    Node*           rootNode        (void);
    void            setStart        (Node& n);
    void            root            (int index);
    void            root            (Node& n);
    void            root            (void);
    void            unroot          (void);
    void            traverse        (void);
    void            traverse        (std::vector<Node*> &inorder);
    Node*           postorder       (int index);
    Node*           postorderIntern (int index);
    Node*           preorder        (int index);
    Node*           preorderIntern  (int index);
    Node*           tip             (int index);
    Node*           taxon           (int index); // Returns a pointer to a terminal in the dataset, regardless of whether or not it is currently placed in the tree.
    void            putInOutgroup   (int index);
    void            putInIngroup    (int index);
    void            prepStepwise    (int left, int right, int anc);
    std::string     writeNewick     (void);
    void            markUniquely    (void);
    void            removeBranch    (Node& subtr);
    void            connectBranch   (Node& subtr, Node& tgt);
    void            connectBranch   (int subtrIndex, int tgtIndex);
    void            tempInsert      (Node& src, Node& tgt);
    void            undoTempInsert  (Node& src);
    
private:
    
    Node*           markDownpass    (int index);
    void            connectDummy    (void);
    
};

#endif /* Tree_hpp */
