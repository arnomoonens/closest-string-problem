//
//  aco.cpp
//  ClosestStringProblem
//
//  Created by Arno Moonens on 9/05/17.
//  Copyright © 2017 Arno Moonens. All rights reserved.
//

#include "aco.hpp"

ACO::ACO(double pbeta, double prho, long int pseed) {
    beta = pbeta;
    rho = prho;
    seed = pseed;
}

/** Heuristic information
 Number of strings that have character x at idx **/
double ACO::heuristic_information(Ant *current_ant, int idx, char x) {
    int count = 0;
    int n = inst->getNumberOfStrings();
    char ** strings = inst->getStrings();
    for (int i = 0; i < n; i++) {
        if (strings[i][idx] == x) {
            count++;
        }
    }
    return (double) count / (double) n;
}

/** Construction (SROM) phase of aco **/
void ACO::construct(Ant *current_ant) {
    int i, j, choice, char_idx;
    char * alphabet = inst->getAlphabet();
    
    int string_length = inst->getStringLength();
    int * string_indices = (int *) malloc(string_length * sizeof(int));
    double sum_prob = 0;
    double * selection_prob = (double *) malloc(string_length * sizeof(double));
    for (i = 0; i < string_length; i++) { // While string is not complete yet
        for(j = 0; j < inst->getAlphabetSize(); j++) {
            sum_prob += pheromone_trails[j][i]*pow(heuristic_information(current_ant, i, alphabet[j]), beta);
            selection_prob[j] = sum_prob;
        }
        choice = ran01(&seed) * sum_prob;
        char_idx = 0;
        while (choice > selection_prob[char_idx]) char_idx++;
        string_indices[i] = char_idx;
    }
    current_ant->setString(string_indices);
    free((void *) string_indices);
    free((void *) selection_prob);
    int sq = current_ant->calculateSolutionQuality();
    current_ant->setSolutionQuality(sq);
    return;
}

/** Updating of pheromone trails of sets **/
void ACO::update_pheromone_trails(Ant *global_best, double tau_min, double tau_max) {
    int char_idx;
    int string_length = inst->getStringLength();
//    double delta_tau = (double) 1 - ((double) global_best->getSolutionQuality() / (double) string_length);
    int * string_indices = global_best->getStringIndices();
    for (int i = 0; i < string_length; i++) {
        char_idx = string_indices[i];
        pheromone_trails[char_idx][i] = ((1.0 - rho) * pheromone_trails[char_idx][i]) + rho * tau_max; // rho * tau_max instead of delta_tau
        if (pheromone_trails[char_idx][i] < tau_min) {
            pheromone_trails[char_idx][i] = tau_min;
        } else if (pheromone_trails[char_idx][i] > tau_max) {
            pheromone_trails[char_idx][i] = tau_max;
        }
    }
    return;
}

void ACO::local_search(Ant * ant) {
    int sq, orig_char_idx;
    int alphabet_size = inst->getAlphabetSize();
//    char * alphabet = inst->getAlphabet();
//    char * string = ant->getString();
//    int n_strings = inst->getStringLength();
//    char ** strings = inst->getStrings();
    int string_length = inst->getStringLength();
    for (int i = 0; i < string_length; i++) { // For every char in the string
        for (int j = 0; j < alphabet_size; j++) { // For every char in the alphabet
            orig_char_idx = ant->getStringIndices()[i];
            if (orig_char_idx == j) continue;
            // Check influence of change and apply if improvement
            ant->setCharacter(i, j);
            sq = ant->calculateSolutionQuality();
            if (sq < ant->getSolutionQuality()) {
                ant->setSolutionQuality(sq);
            } else {
                ant->setCharacter(i, orig_char_idx);
            }
        }
    }
    return;
}

/** Execute aco algorithm **/
Solution * ACO::execute(Instance *instance, bool (*termination_criterion)(Solution *), void (*notify_improvement)(Solution *), long int nants) {
    int i;
    bool improvement;
    inst = instance;
    Ant *global_best = NULL;
    Ant **ants = (Ant **) malloc(nants * sizeof(Ant *));
    int alphabet_size = inst->getAlphabetSize();
    int string_length = inst->getStringLength();
    double tau_max = (double) 1 / (double) alphabet_size;
    double tau_min = tau_max / ((double) alphabet_size * (double) string_length);
    pheromone_trails = (double **) malloc(alphabet_size * sizeof(double *));
    for (i = 0; i < alphabet_size; i++) {
        pheromone_trails[i] = (double *) malloc(string_length * sizeof(double));
        for (int j = 0; j < string_length; j++) pheromone_trails[i][j] = tau_max;
    }
    while(!termination_criterion(global_best)) {
        improvement = false;
        for (i = 0; i < nants; i++) { // For each ant...
            ants[i] = new Ant(inst);
            construct(ants[i]); // Construct a solution...
            local_search(ants[i]); /// And apply local search
            if (!global_best) {
                global_best = ants[i];
                improvement = true;
                notify_improvement(ants[i]);
            } else if (ants[i]->getSolutionQuality() <= global_best->getSolutionQuality()) {
                if (ants[i]->getSolutionQuality() < global_best->getSolutionQuality()) notify_improvement(ants[i]);
                delete global_best;
                global_best = ants[i];
                improvement = true;
            } else {
                delete ants[i];
            }
        }
        if (improvement) {
            tau_max = (double) 1 / (double) global_best->getSolutionQuality();
            tau_min = tau_max / ((double) alphabet_size * (double) string_length);
        }
        update_pheromone_trails(global_best, tau_min, tau_max);
    }
    // free all arrays
    for (i = 0; i < alphabet_size; i++) free((void *) pheromone_trails[i]);
    free((void *) pheromone_trails);
    free((void *) ants);
    return global_best;
}
