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
    int k; // alphabet size
    int n; // number of strings
    int l; // string length
    char * alphabet; // k characters
    char ** strings; // n strings
    
public:
    Instance(const char * csp_file_name);
    ~Instance();
    
    void print();
};

#endif /* instance_hpp */
