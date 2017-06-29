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


Node* NewickReader::traverseNewick(std::string Newick, int *index)
{
    Node* n = _tree.newVertex();
    
    assert(n != NULL);
    
    Node* p = NULL;
    
    std::cout << Newick[*index];
    
    ++(*index);
    do {
        
        if (Newick[*index] == '(') {
            p = traverseNewick(Newick, index);
            assert(p != NULL);
            n->addDescendant(*p);
            p = NULL;
        }
        
        if (isalnum(Newick.at(*index))) {
            std::string label;
            do {
                std::cout << Newick[*index];
                label.push_back(Newick[*index]);
                ++(*index);
            } while (Newick[*index] != ',' && Newick[*index] != ')');
            
            int tipindex = 0;
            
            if(std::all_of(label.begin(), label.end(), ::isdigit)) {
                tipindex = std::stoi(label);
            }
            else {
                // TODO: lookup the label's index
            }
            
            p = _tree.newTip(tipindex);
            assert(p != NULL);
            n->addDescendant(*p);
            p = NULL;
        }
        
        if (Newick[*index] == ',') {
            std::cout << Newick[*index];
            ++(*index);
        }
        
    } while (Newick[*index] != ')');
    
    std::cout << Newick[*index];
    ++(*index);
    //std::cout << Newick[*index];
    
    return n;
}

void NewickReader::read(std::string Newick, bool wnames, bool rooted)
{
    if (checkValid(Newick)) {
        return;
    }
    
    _tree.reset();
    
    int index = 0;
    Node* s = traverseNewick(Newick, &index);
    
    std::cout << std::endl;
    _tree.setStart(*s);
    _tree.traverse();
    
    std::cout << std::endl;
    
    return;
}

void NewickReader::read(char *Newick, bool wnames, bool rooted)
{
    read(std::string(Newick), wnames, rooted);
}
