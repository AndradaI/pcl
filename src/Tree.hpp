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

#include "Node.hpp"
#include "BinNode.hpp"

class Topology;

class Tree {
    
protected:
    int                             _num_taxa;
    std::vector<Node*>              _postorder;
    Node*                           _start;
    unsigned long                   _natural_score;
    double                          _real_score;
    bool                            _isrooted;
    
private:
    std::vector<Node*>              _nodes;
    std::vector<Node*>              _tips;
    std::vector<Node*>              _internals;
    std::vector<Node*>              _outgroup;
    std::vector<Node*>::iterator    _nextFreeTip;
    std::vector<Node*>::iterator    _nextFreeInternal;
    std::vector<Node*>::iterator    _nextFreeOutgroup;
    
public:
    
    friend class Topology;
    
    Tree ()
    {
        
    }
    
    Tree(int numtaxa)
    :   _start(NULL),
        _natural_score(0),
        _real_score(0.0)
    {
        int num_nodes = 2 * numtaxa;
        _num_taxa = numtaxa;
        
        _nodes.reserve(num_nodes);
        
        Node* newnode = NULL;
        int index = 0;
        
        for (index = 0; index < num_nodes; ++index) {
            newnode = NULL;
            if (index < numtaxa) {
                newnode = new BinNode(index, index + 1);
                //_tips.push_back(*it);
            } else {
                newnode = new BinNode(index, 0);
                //_internals.push_back(*it);
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
    
    bool            isrooted    (void);
    unsigned int    size        (void);
    void            restore     (Topology& topol);
    void            reset       (void);
    void            incrScore   (unsigned long s);
    void            incrScore   (double s);
    void            setScore    (double s);
    void            setScore    (unsigned long s);
    void            clearScore  (void);
    unsigned long   natScore    (void);
    double          realScore   (void);
    Node*           newTip      (int index);
    Node*           newVertex   (void);
    Node&           node        (int index);
    Node&           rootNode    (void);
    void            setStart    (Node& n);
    void            root        (int index);
    void            unroot      (void);
    void            traverse    (void);
    std::string     writeNewick (void);
    
private:
    
};

#endif /* Tree_hpp */
