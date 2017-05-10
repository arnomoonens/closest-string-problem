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
    int string_length = inst->getStringLength();
    int n_strings = inst->getNumberOfStrings();
    string = (char *) malloc(string_length * sizeof(char));
    string_indices = (int *) malloc(string_length * sizeof(int));
    string_distances = (int *) malloc(n_strings * sizeof(int));
    for (int i = 0; i < n_strings; i++) string_distances[i] = 0;
}

Solution::~Solution() {
    free((void *) string);
    free((void *) string_indices);
    free((void *) string_distances);
}

void Solution::setString(int * indices) {
    int string_length = inst->getStringLength();
    char * alphabet = inst->getAlphabet();
    memcpy(string_indices, indices, string_length * sizeof(int));
    for (int i = 0; i < string_length; i++) {
        string[i] = alphabet[indices[i]];
    }
    return;
}

int Solution::calculateSolutionQuality() {
    int result = 0;
    char ** strings = inst->getStrings();
    for (int i = 0; i < inst->getStringLength(); i++) {
        for (int j = 0; j < inst->getNumberOfStrings(); j++) {
            if (string[i] != strings[j][i]) {
                string_distances[j]++;
                if (string_distances[j] > result) result = string_distances[j];
            }
        }
    }
    return result;
}

void Solution::setCharacter(int idx, char ch) {
    char * alphabet = inst->getAlphabet();
    string[idx] = ch;
    for (int i = 0; i < inst->getAlphabetSize(); i++) {
        if (alphabet[i] == ch) {
            string_indices[idx] = i;
            break;
        }
    }
    return;
}

void Solution::setCharacter(int idx, int char_idx) {
    string_indices[idx] = char_idx;
    string[idx] = inst->getAlphabet()[char_idx];
    return;
}
