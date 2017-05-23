//
//  aco.cpp
//  ClosestStringProblem
//
//  Created by Arno Moonens on 20/05/17.
//  Copyright © 2017 Arno Moonens. All rights reserved.
//

#include "aco.hpp"


ACO::ACO(double pbeta, double prho, long int pseed) {
    beta = pbeta;
    rho = prho;
    seed = pseed;
}

ACO::~ACO() {
}


void ACO::calculate_probability() {
    long int i, j;
    long int alphabet_size = inst->getAlphabetSize();
    long int string_length = inst->getStringLength();
    for (i = 0; i < alphabet_size; i++) {
        for (j = 0; j < string_length; j++) {
            probability[i][j] = pheromone_trails[i][j] * pow(heuristic_information(j, i), beta);
        }
    }
}
