//
//  NewickReader.hpp
//  Phylogenetic Class Library
//
//  Created by mbrazeau on 28/06/2017.
//  Copyright © 2017 Brazeau Lab. All rights reserved.
//

#ifndef NewickReader_hpp
#define NewickReader_hpp

#include <string>
#include <vector>

#include "Topology.hpp"

class NewickReader {
    
    int         _numtaxa;
    std::string _Newick;
    
public:
    
    NewickReader(int numtaxa)
    : _numtaxa(numtaxa)
    {
        
    }
    
    void        read(std::string Newick, bool wnames, bool rooted);
    void        read(char* Newick, bool wnames, bool rooted);
    Topology&   getTopol(void);
    
private:
    
    std::string::iterator travNewick(std::string::iterator n, std::vector<int> &edges);
    bool checkValid(std::string Newick);
    
};

#endif /* NewickReader_hpp */
