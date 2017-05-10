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
    int string_length = inst->getStringLength();
    int n_strings = inst->getNumberOfStrings();
    string_max_dist_index = -1;
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
    calculateSolutionQuality();
    return;
}

void Solution::calculateSolutionQuality2() {
    fx = 0;
    char ** strings = inst->getStrings();
    for (int i = 0; i < inst->getNumberOfStrings(); i++) {
        string_distances[i] = 0;
        for (int j = 0; j < inst->getStringLength(); j++) {
            if (string[j] != strings[i][j]) string_distances[i]++;
        }
        if (string_distances[i] > fx) fx = string_distances[i];
    }
    return;
}

void Solution::calculateSolutionQuality() {
    int char_idx, j, m, string_idx;
    fx = 0;
    int ** strings_per_char_count = inst->getStringsPerCharCount();
    int *** char_to_string = inst->getCharToString();
    for (int i = 0; i < inst->getStringLength(); i++) {
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

void Solution::setCharacter(int idx, char ch) {
    std::cout << "Don't use this function" << std::endl;
//    char * alphabet = inst->getAlphabet();
//    string[idx] = ch;
//    for (int i = 0; i < inst->getAlphabetSize(); i++) {
//        if (alphabet[i] == ch) {
//            string_indices[idx] = i;
//            break;
//        }
//    }
    return;
}

void Solution::setCharacter(int idx, int char_idx) {
    int i;
    int orig_char_idx = string_indices[idx];
    string_indices[idx] = char_idx;
    int *** char_to_string = inst->getCharToString();
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
