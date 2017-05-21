//
//  acs.cpp
//  ClosestStringProblem
//
//  Created by Arno Moonens on 20/05/17.
//  Copyright © 2017 Arno Moonens. All rights reserved.
//

#include "acs.hpp"

ACS::ACS(double pbeta, double prho, double pexploitation_prob, long int pseed) : ACO(pbeta, prho, pseed) {
    exploitation_prob = pexploitation_prob;
}

ACS::~ACS() {
}

/** Heuristic information
 Number of strings that have character x at idx **/
double ACS::heuristic_information(Ant *current_ant, long int idx, long int char_idx) {
    return (double) inst->getStringsPerCharCount()[idx][char_idx];
}

/** Construction (SROM) phase of aco **/
void ACS::construct(Ant *current_ant) {
    long int i, j, char_idx;
    double sum_prob, choice, q, max_character_score;
    long int string_length = inst->getStringLength();
    long int alphabet_size = inst->getAlphabetSize();
    long int * string_indices = (long int *) malloc(string_length * sizeof(long int));
    double * selection_prob = (double *) malloc(alphabet_size * sizeof(double));
    double * character_score = (double *) malloc(alphabet_size * sizeof(double));
    for (i = 0; i < string_length; i++) { // While string is not complete yet
        sum_prob = 0;
        for(j = 0; j < alphabet_size; j++) {
            character_score[j] = pheromone_trails[j][i] * pow(heuristic_information(current_ant, i, j), beta);
            sum_prob += character_score[j];
            selection_prob[j] = sum_prob;
        }
        q = ran01(&seed);
        char_idx = 0;
        if (q < exploitation_prob) { // Exploitation: Choose the "best" one
            max_character_score = 0;
            for (j = 0; j < alphabet_size; j++) {
                if (character_score[j] > max_character_score) {
                    max_character_score = character_score[j];
                    char_idx = j;
                }
            }
        } else { // Exploration
            choice = ran01(&seed) * sum_prob;
            while (choice > selection_prob[char_idx]) char_idx++;
        }
        string_indices[i] = char_idx;
    }
    current_ant->setString(string_indices);
    free((void *) string_indices);
    free((void *) selection_prob);
    free((void *) character_score);
    return;
}

/** Updating of pheromone trails of sets **/
void ACS::update_pheromone_trails(Ant *global_best, double tau_min, double tau_max) {
    long int char_idx;
    long int string_length = inst->getStringLength();
    //    double delta_tau_max = (double) 1 - ((double) global_best->getSolutionQuality() / (double) string_length);
    double delta_tau = rho * tau_max;
    long int * string_indices = global_best->getStringIndices();
    for (int i = 0; i < string_length; i++) {
        char_idx = string_indices[i];
        pheromone_trails[char_idx][i] = (1.0 - rho) * pheromone_trails[char_idx][i] + delta_tau;
        // Should the trails be limited or not?
        //        if (pheromone_trails[char_idx][i] < tau_min) {
        //            pheromone_trails[char_idx][i] = tau_min;
        //        } else if (pheromone_trails[char_idx][i] > tau_max) {
        //            pheromone_trails[char_idx][i] = tau_max;
        //        }
    }
    return;
}


/** Execute aco algorithm **/
Solution * ACS::execute(Instance *instance, bool (*termination_criterion)(Solution *), void (*notify_improvement)(Solution *), long int nants) {
    long int i;
    bool improvement;
    inst = instance;
    Ant *global_best = NULL;
    Ant **ants = (Ant **) malloc(nants * sizeof(Ant *));
    long int alphabet_size = inst->getAlphabetSize();
    long int string_length = inst->getStringLength();
    double tau_max = (double) 1 / (double) alphabet_size;
    double tau_min = tau_max / ((double) alphabet_size * (double) string_length);
    pheromone_trails = (double **) malloc(alphabet_size * sizeof(double *));
    for (i = 0; i < alphabet_size; i++) {
        pheromone_trails[i] = (double *) malloc(string_length * sizeof(double));
        for (long int j = 0; j < string_length; j++) pheromone_trails[i][j] = tau_max;
    }
    while(!termination_criterion(global_best)) {
        improvement = false;
        for (i = 0; i < nants; i++) { // For each ant...
            ants[i] = new Ant(inst);
            construct(ants[i]); // Construct a solution...
            if (!global_best) {
                global_best = ants[i];
                improvement = true;
                notify_improvement(ants[i]);
            } else if (ants[i]->getSolutionQuality() <= global_best->getSolutionQuality()) {
                if (ants[i]->getSolutionQuality() < global_best->getSolutionQuality()) {
                    notify_improvement(ants[i]);
                    improvement = true;
                }
                delete global_best;
                global_best = ants[i];
            } else {
                delete ants[i];
            }
        }
        //        if (improvement) {
        //            tau_max = (double) 1 / (double) global_best->getSolutionQuality();
        //            tau_min = tau_max / ((double) alphabet_size * (double) string_length);
        //        }
        update_pheromone_trails(global_best, tau_min, tau_max);
    }
    // free all arrays
    for (i = 0; i < alphabet_size; i++) free((void *) pheromone_trails[i]);
    free((void *) pheromone_trails);
    free((void *) ants);
    return global_best;
}
