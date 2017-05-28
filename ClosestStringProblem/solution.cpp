//
//  solution.cpp
//  ClosestStringProblem
//
//  Created by Arno Moonens on 8/05/17.
//  Copyright © 2017 Arno Moonens. All rights reserved.
//

#include "solution.hpp"


Solution::Solution(Instance * pinst) {
    inst = pinst;
    fx = 0;
    long int string_length = inst->getStringLength();
    long int n_strings = inst->getNumberOfStrings();
    string = (char *) malloc(string_length * sizeof(char));
    string_indices = (long int *) malloc(string_length * sizeof(long int));
    string_distances = (long int *) malloc(n_strings * sizeof(long int));
    for (long int i = 0; i < n_strings; i++) string_distances[i] = 0;
}

Solution::~Solution() {
    free((void *) string);
    free((void *) string_indices);
    free((void *) string_distances);
}

void Solution::setString(long int * indices) {
    long int string_length = inst->getStringLength();
    char * alphabet = inst->getAlphabet();
    memcpy(string_indices, indices, string_length * sizeof(long int));
    for (long int i = 0; i < string_length; i++) {
        string[i] = alphabet[indices[i]];
    }
    return;
}


void Solution::computeSolutionQuality() {
    long int i, j;
    long int number_of_strings = inst->getNumberOfStrings();
    char ** strings = inst->getStrings();
    long int string_length = inst->getStringLength();
    fx = 0;
    for (i = 0; i < number_of_strings; i++) {
        for (j = 0; j < string_length; j++) {
            if (strings[i][j] != string[j]) {
                string_distances[i]++;
            }
        }
        if (string_distances[i] > fx) fx = string_distances[i];
    }
}

/** Add a character to the solution **/
void Solution::addCharacter(long int idx, long int char_idx) {
    string_indices[idx] = char_idx;
    string[idx] = inst->getAlphabet()[char_idx];
    return;
}

void Solution::setCharacter(long int idx, long int new_char_idx) {
    long int i, char_idx;
    char * alphabet = inst->getAlphabet();
    char ** strings = inst->getStrings();
    long int orig_char_idx = string_indices[idx];
    long int number_of_strings = inst->getNumberOfStrings();
    string_indices[idx] = new_char_idx;
    string[idx] = alphabet[new_char_idx];
    for (i = 0; i < number_of_strings; i++) {
        char_idx = 0;
        while (strings[i][idx] != alphabet[char_idx]) char_idx++;
        if (char_idx == orig_char_idx) {
            string_distances[i]++;
        } else if(char_idx == new_char_idx) {
            string_distances[i]--;
        }
    }
    fx = 0;
    for (i = 0; i < number_of_strings; i++) {
        if (string_distances[i] > fx) {
            fx = string_distances[i];
        }
    }
    return;
}

void Solution::setCharacter(long int idx, char ch) {
    std::cout << "Don't use this function" << std::endl;
    char * alphabet = inst->getAlphabet();
    long int char_idx = 0;
    while (ch != alphabet[char_idx]) char_idx++;
    setCharacter(idx, char_idx);
    return;
}
