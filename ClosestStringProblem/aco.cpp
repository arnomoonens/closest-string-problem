//
//  aco.cpp
//  ClosestStringProblem
//
//  Created by Arno Moonens on 20/05/17.
//  Copyright © 2017 Arno Moonens. All rights reserved.
//

#include "aco.hpp"


ACO::ACO(Instance * pinst, double prho, long int pseed) {
    inst = pinst;
    rho = prho;
    seed = pseed;
    long int alphabet_size = inst->getAlphabetSize();
    long int string_length = inst->getStringLength();
    pheromone_trails = (double **) malloc(alphabet_size * sizeof(double *));
    probability = (double **) malloc(alphabet_size * sizeof(double *));
    for (long int i = 0; i < alphabet_size; i++) {
        pheromone_trails[i] = (double *) malloc(string_length * sizeof(double));
        probability[i] = (double *) malloc(string_length * sizeof(double));
    }
}

ACO::~ACO() {
    long int alphabet_size = inst->getAlphabetSize();
    long int string_length = inst->getStringLength();
    for (long int i = 0; i < alphabet_size; i++) {
        free((void *) pheromone_trails[i]);
        free((void *) probability[i]);
    }
    for (long int i = 0; i < string_length; i++) {
        free((void *) heuristic_information[i]);
    }
    free((void *) heuristic_information);
    free((void *) pheromone_trails);
    free((void *) probability);
}

void ACO::compute_heuristic_information(Instance * inst) {
    long int i, j;
    long int char_idx;
    char * alphabet = inst->getAlphabet();
    char ** strings = inst->getStrings();
    long int string_length = inst->getStringLength();
    long int alphabet_size = inst->getAlphabetSize();
    long int number_of_strings = inst->getNumberOfStrings();
    heuristic_information = (double **) malloc(string_length * sizeof(double *));
    for (i = 0; i < string_length; i++) {
        heuristic_information[i] = (double *) malloc(alphabet_size * sizeof(double));
        for (j = 0; j < alphabet_size; j++) {
            heuristic_information[i][j] = 0;
        }
    }
    for (i = 0; i < number_of_strings; i++) {
        for (j = 0; j < string_length; j++) {
            char_idx = 0;
            while(strings[i][j] != alphabet[char_idx]) char_idx++;
            heuristic_information[j][char_idx]++;
        }
    }
}

