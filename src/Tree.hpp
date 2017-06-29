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

#include "Node.hpp"
#include "BinNode.hpp"

class Topology;

class Tree {
    
    int                 _num_taxa;
    std::vector<Node*>  _nodes;
    std::vector<Node*>  _postorder;
    std::vector<Node*>  _tips;
    std::vector<Node*>  _internals;
    Node*               _start;
    unsigned long       _natural_score;
    double              _rational_score;
    
public:
    
    Tree(int numtaxa)
    : _start(NULL),
    _natural_score(0),
    _rational_score(0.0)
    {
        int num_nodes = 2 * numtaxa;
        
        _nodes.reserve(num_nodes);
        
        std::vector<Node*>::iterator it;
        
        int index = 0;
        
        for (it = _nodes.begin(); it != _nodes.end(); ++it) {
            if (index < numtaxa) {
                *it = new BinNode(index, index + 1);
            } else {
                *it = new BinNode(0, index + 1);
            }
            ++index;
        }
    }
    
    ~Tree()
    {
        std::vector<Node*>::iterator it;
        
        for (it = _nodes.begin(); it != _nodes.end(); ++it)
        {
            delete *it;
        }
    }
    
    void        restore     (Topology& topol);
    void        incrScore   (unsigned long s);
    void        incrScore   (double s);
    int         natScore    (void);
    double      realScore   (void);
    Node&       node        (int index);
    Node&       rootNode    (void);
    void        root        (int index);
    void        unroot      (void);
    void        traverse    (void);
    std::string writeNewick (void);
};

#endif /* Tree_hpp */
