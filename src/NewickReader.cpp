//
//  NewickReader.cpp
//  Phylogenetic Class Library
//
//  Created by mbrazeau on 28/06/2017.
//  Copyright © 2017 Brazeau Lab. All rights reserved.
//

#include "NewickReader.hpp"

// TODO: TEMP
#include <iostream>

bool NewickReader::checkValid(std::string Newick)
{
    bool ret = false;
    
    bool spaceOK = false;
    
    std::string::iterator it;
    
    int open        = 0;
    int close       = 0;
    int comma       = 0;
    int badspace    = 0;
    
    it = Newick.begin();
    
    if (*(Newick.end()-1) != ';') {
        return true;
    }
    
    while (it != Newick.end()) {
        
        if (*it == '(') {
            ++open;
        }
        
        if (*it == '\'' || open == 0) {
            if (spaceOK == false) {
                spaceOK = true;
            }
            else {
                spaceOK = false;
            }
        }
        
        if (*it == ' ') {
            if (spaceOK == false) {
                ++badspace;
            }
        }
       
        if (*it == ')') {
            ++close;
        }
        
        if (*it == ',') {
            ++comma;
        }
        
        ++it;
    }
    
    if (badspace) {
        return true;
    }
    
    if (open != close) {
        return true;
    }
    
    return ret;
}


Node* NewickReader::traverseNewick(std::string &Newick, int *index, bool isrooted)
{
    Node* n = NULL;
    if (*index == 0)
    {
        n = _tree.rootNode();
    }
    else {
        n = _tree.newVertex();
    }
    
    assert(n != NULL);
    
    Node* p = NULL;
    
    ++(*index);
    
    do {
        
        if (Newick[*index] == '(') {
            p = traverseNewick(Newick, index, isrooted);
            assert(p != NULL);
            n->addDescendant(*p);
            p = NULL;
        }
        
        if (isalnum(Newick.at(*index))) {
            std::string label;
            do {
                label.push_back(Newick[*index]);
                ++(*index);
            } while (Newick[*index] != ',' && Newick[*index] != ')');
            
            int tipindex = 0;
            
            if(std::all_of(label.begin(), label.end(), ::isdigit)) {
                tipindex = std::stoi(label);
            }
            else {
                // TODO: lookup the label's index
                // Get that index
                // Copy label into node
            }
            
            p = _tree.node(tipindex-1);//_tree.newTip(tipindex);
            assert(p != NULL);
            n->addDescendant(*p);
            p = NULL;
        }
        
        if (Newick[*index] == ',') {
            ++(*index);
        }
        
    } while (Newick[*index] != ')');
    
    ++(*index);
    
    return n;
}

void NewickReader::read(std::string Newick, bool wnames, bool rooted)
{
    if (checkValid(Newick)) {
        return;
    }
    
    _tree.reset();
    
    int index = 0;
    Node* s = traverseNewick(Newick, &index, rooted);

#ifdef DEBUG
    std::cout << std::endl;
#endif
    
    _tree.setStart(*s);
    
    if (rooted == true) {
        _tree.root();
    } else {
        _tree.unroot();
    }
    
    _tree.traverse();
    
    //#ifdef DEBUG
    std::cout << std::endl;
    //#endif
    
    return;
}

void NewickReader::read_cstr(char *Newick, bool wnames, bool rooted)
{
    read(std::string(Newick), wnames, rooted);
}

Topology& NewickReader::getTopol(void)
{
    Topology* topol = new Topology(_numtaxa);
    
    topol->store(_tree);
    
    return *topol;
}
