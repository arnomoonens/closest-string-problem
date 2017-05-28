//
//  instance.hpp
//  ClosestStringProblem
//
//  Created by Arno Moonens on 8/05/17.
//  Copyright © 2017 Arno Moonens. All rights reserved.
//

#ifndef instance_hpp
#define instance_hpp

#include <iostream>
#include <stdio.h>

#include "utils.hpp"

class Instance {
    FILE *instance_file; // File with the instance specification
    long int k; // alphabet size
    long int n; // number of strings
    long int l; // string length
    char * alphabet; // k characters
    char ** strings; // n strings
    
public:
    Instance(const char * csp_file_name);
    ~Instance();
    long int getAlphabetSize() {return k;};
    long int getNumberOfStrings() {return n;};
    long int getStringLength() {return l;};
    char * getAlphabet() {return alphabet;};
    char ** getStrings() {return strings;};
    void print();
};

#endif /* instance_hpp */
