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
    fx = -1;
    long int string_length = inst->getStringLength();
    long int n_strings = inst->getNumberOfStrings();
    string_max_dist_index = -1;
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

void Solution::calculateSolutionQuality() {
    fx = 0;
//    char ** strings = inst->getStrings();
    for (long int i = 0; i < inst->getNumberOfStrings(); i++) {
//        string_distances[i] = 0;
//        for (long int j = 0; j < inst->getStringLength(); j++) {
//            if (string[j] != strings[i][j]) string_distances[i]++;
//        }
        if (string_distances[i] > fx) fx = string_distances[i];
    }
    return;
}

void Solution::calculateSolutionQuality2() {
    long int char_idx, j, m, string_idx;
    fx = 0;
    long int ** strings_per_char_count = inst->getStringsPerCharCount();
    long int *** char_to_string = inst->getCharToString();
    for (long int i = 0; i < inst->getStringLength(); i++) {
        char_idx = string_indices[i];
        for (j = 0; j < inst->getAlphabetSize(); j++) {
            if (j == char_idx) continue;
            for (m = 0; m < strings_per_char_count[i][j]; m++) {
                string_idx = char_to_string[i][j][m];
                string_distances[string_idx]++;
                if (string_distances[string_idx] > fx) fx = string_distances[string_idx];
            }
        }
    }
    return;
}

void Solution::setCharacter(long int idx, long int char_idx) {
    long int i;
    long int orig_char_idx = string_indices[idx];
    string_indices[idx] = char_idx;
    long int *** char_to_string = inst->getCharToString();
    string[idx] = inst->getAlphabet()[char_idx];
    for (i = 0; i < inst->getStringsPerCharCount()[idx][orig_char_idx]; i++)
        string_distances[char_to_string[idx][orig_char_idx][i]]++;
    for (i = 0; i < inst->getStringsPerCharCount()[idx][char_idx]; i++)
        string_distances[char_to_string[idx][char_idx][i]]--;
    fx = 0;
    for (i = 0; i < inst->getNumberOfStrings(); i++) {
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
