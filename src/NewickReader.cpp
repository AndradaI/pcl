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

std::string::iterator
NewickReader::travNewick
(std::string::iterator n, std::vector<int> &edges)
{
    do {
        ++n;
        
        if (*n == '(') {
            n = travNewick(n, edges);
        }
    } while (*n != ')');
    
    return n;
}

void NewickReader::read(std::string Newick, bool wnames, bool rooted)
{
    if (checkValid(Newick)) {
        return;
    }
    
    std::vector<int> edges(_numtaxa);
    
    std::cout << edges.size() << "\n";
    
    travNewick(Newick.begin(), edges);
    std::cout << edges.size() << "\n";
}

void NewickReader::read(char *Newick, bool wnames, bool rooted)
{
    read(std::string(Newick), wnames, rooted);
}
