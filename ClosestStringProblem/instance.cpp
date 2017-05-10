//
//  instance.cpp
//  ClosestStringProblem
//
//  Created by Arno Moonens on 8/05/17.
//  Copyright © 2017 Arno Moonens. All rights reserved.
//

#include "instance.hpp"

Instance::Instance(const char * inst_file_name) {
    int i, j;
    instance_file = fopen(inst_file_name, "r");
    
    if (fscanf(instance_file,"%d",&k)!=1)
        error_reading_file("ERROR: there was an error reading instance file.");
    if (fscanf(instance_file,"%d",&n)!=1)
        error_reading_file("ERROR: there was an error reading instance file.");
    if (fscanf(instance_file,"%d",&l)!=1)
        error_reading_file("ERROR: there was an error reading instance file.");

    alphabet = (char *) malloc(k * sizeof(char));
    for (i = 0; i < k; i++) {
        if (fscanf(instance_file,"%s",&alphabet[i])!=1)
            error_reading_file("ERROR: there was an error reading instance file.");
    }
    
    strings  = (char **) malloc(n*sizeof(char *));
    for (i = 0; i < n; i++) {
        strings[i] = (char *) malloc(l*sizeof(char));
        if (fscanf(instance_file,"%s",strings[i])!=1)
            error_reading_file("ERROR: there was an error reading instance file.");
    }
    
    strings_per_char_count = (int **) malloc(l * sizeof(char *));
    for (i = 0; i < l; i++) {
        strings_per_char_count[i] = (int *) malloc(k * sizeof(char));
        for (j = 0; j < k; j++) strings_per_char_count[i][j] = 0;
    }
    
    char_to_string = (int ***) malloc(l * sizeof(int **));
    for (i = 0; i < l; i++) {
        char_to_string[i] = (int **) malloc(k * sizeof(int *));
        for (j = 0; j < k; j++) {
            char_to_string[i][j] = (int *) malloc(n * sizeof(int));
        }
    }
    
    for (i = 0; i < n; i++) {
        for (j = 0; j < l; j++) {
            int char_idx = 0;
            while(strings[i][j] != alphabet[char_idx]) char_idx++;
            int next_idx = strings_per_char_count[j][char_idx];
            char_to_string[j][char_idx][next_idx] = i;
            strings_per_char_count[j][char_idx]++;
        }
    }
    
    return;
}

Instance::~Instance() {
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
