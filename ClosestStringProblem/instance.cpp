//
//  instance.cpp
//  ClosestStringProblem
//
//  Created by Arno Moonens on 8/05/17.
//  Copyright © 2017 Arno Moonens. All rights reserved.
//

#include "instance.hpp"

Instance::Instance(const char * inst_file_name) {
    instance_file = fopen(inst_file_name, "r");
    
    if (fscanf(instance_file,"%d",&k)!=1)
        error_reading_file("ERROR: there was an error reading instance file.");
    if (fscanf(instance_file,"%d",&n)!=1)
        error_reading_file("ERROR: there was an error reading instance file.");
    if (fscanf(instance_file,"%d",&l)!=1)
        error_reading_file("ERROR: there was an error reading instance file.");

    alphabet = (char *) malloc(k * sizeof(char));
    for (int i = 0; i < k; i++) {
        if (fscanf(instance_file,"%s",&alphabet[i])!=1)
            error_reading_file("ERROR: there was an error reading instance file.");
    }
    
    strings  = (char **) malloc(n*sizeof(char *));
    for (int i = 0; i < n; i++) {
        strings[i] = (char *) malloc(l*sizeof(char));
        if (fscanf(instance_file,"%s",strings[i])!=1)
            error_reading_file("ERROR: there was an error reading instance file.");
    }
    return;
}

Instance::~Instance() {
    std::cout << "freeing" << std::endl;
    free(alphabet);
    for (int i = 0; i < n; i++) {
        free((void *) strings[i]);
    }
    free((void *) strings);
}

void Instance::print() {
    std::cout << "CSP instance" << std::endl
    << "Alphabet size: " << k << std::endl
    << "Number of strings: " << n << std::endl
    << "String length: " << l << std::endl
    << "Alphabet: " << alphabet << std::endl;
    return;
}
