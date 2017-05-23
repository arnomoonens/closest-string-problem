//
//  acs.cpp
//  ClosestStringProblem
//
//  Created by Arno Moonens on 20/05/17.
//  Copyright © 2017 Arno Moonens. All rights reserved.
//

#include "acs.hpp"

ACS::ACS(double pbeta, double prho, double pexploitation_prob, bool puse_local_search, long int pseed) : ACO(pbeta, prho, pseed) {
    exploitation_prob = pexploitation_prob;
    use_local_search = puse_local_search;
}

ACS::~ACS() {
}

/** Construction phase **/
void ACS::construct(Ant *current_ant) {
    long int i, j, char_idx;
    double sum_prob, choice, q, max_character_score;
    long int string_length = inst->getStringLength();
    long int alphabet_size = inst->getAlphabetSize();
    double * selection_prob = (double *) malloc(alphabet_size * sizeof(double));
    double * character_score = (double *) malloc(alphabet_size * sizeof(double));
    for (i = 0; i < string_length; i++) { // For every position in the new string
        sum_prob = 0;
        for(j = 0; j < alphabet_size; j++) {
            character_score[j] = probability[j][i];
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
        current_ant->addCharacter(i, char_idx);
        local_pheromone_update(current_ant, i, char_idx);
    }
    free((void *) selection_prob);
    free((void *) character_score);
    return;
}

void ACS::local_pheromone_update(Ant *current_ant, long int idx, long int char_idx) {
    pheromone_trails[char_idx][idx] = (1 - rho) * pheromone_trails[char_idx][idx] + rho * tau_init;
    probability[char_idx][idx] = pheromone_trails[char_idx][idx] * pow(heuristic_information(idx, char_idx), beta);
}

/** Updating of pheromone trails of sets **/
void ACS::global_pheromone_update(Ant *global_best) {
    long int char_idx;
    long int string_length = inst->getStringLength();
        double delta_tau = (double) 1 - ((double) global_best->getSolutionQuality() / (double) string_length);
//    double delta_tau = (double) 1.0 / (double) global_best->getSolutionQuality();
    long int * string_indices = global_best->getStringIndices();
    for (int i = 0; i < string_length; i++) {
        char_idx = string_indices[i];
        pheromone_trails[char_idx][i] = (1.0 - rho) * pheromone_trails[char_idx][i] + delta_tau;
        // Should the trails be limited or not?
//                if (pheromone_trails[char_idx][i] < tau_min) {
//                    pheromone_trails[char_idx][i] = tau_min;
//                } else if (pheromone_trails[char_idx][i] > tau_max) {
//                    pheromone_trails[char_idx][i] = tau_max;
//                }
        probability[char_idx][i] = pheromone_trails[char_idx][i] * pow(heuristic_information(i, char_idx), beta); // Update probabilities
    }
    return;
}

void ACS::local_search(Ant * ant) {
    long int orig_char_idx, j;
    long int alphabet_size = inst->getAlphabetSize();
    long int orig_solq = ant->getSolutionQuality();
    long int string_length = inst->getStringLength();
    for (long int i = 0; i < string_length; i++) { // For every char in the string
        for (j = 0; j < alphabet_size; j++) { // For every char in the alphabet
            orig_char_idx = ant->getStringIndices()[i];
            if (orig_char_idx == j) continue;
            // Check influence of change and apply if improvement
            ant->setCharacter(i, j);
            if (ant->getSolutionQuality() < orig_solq) { // Only apply new solution if strictly better
                orig_solq = ant->getSolutionQuality();
            } else {
                ant->setCharacter(i, orig_char_idx);
            }
        }
    }
    return;
}

/** Execute aco algorithm **/
Solution * ACS::execute(Instance *instance, bool (*termination_criterion)(Solution *), void (*notify_improvement)(Solution *), long int nants) {
    long int i, j;
    bool improvement;
    inst = instance;
    Ant *global_best = NULL;
    Ant **ants = (Ant **) malloc(nants * sizeof(Ant *));
    long int alphabet_size = inst->getAlphabetSize();
    long int string_length = inst->getStringLength();
    double tau_max = (double) 1 / (double) alphabet_size;
//    double tau_min = tau_max / ((double) alphabet_size * (double) string_length);
    tau_init = 1.0; // 1.0 as in the paper (instead of tau_max)
    pheromone_trails = (double **) malloc(alphabet_size * sizeof(double *));
    probability = (double **) malloc(alphabet_size * sizeof(double *));
    for (i = 0; i < alphabet_size; i++) {
        pheromone_trails[i] = (double *) malloc(string_length * sizeof(double));
        for (j = 0; j < string_length; j++) pheromone_trails[i][j] = tau_max;
        probability[i] = (double *) malloc(string_length * sizeof(double));
    }
    calculate_probability();
    while(!termination_criterion(global_best)) {
        improvement = false;
        for (i = 0; i < nants; i++) { // For each ant...
            ants[i] = new Ant(inst);
            construct(ants[i]); // Construct a solution...
            if (use_local_search) local_search(ants[i]); // Apply local search if needed
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
        global_pheromone_update(global_best);
    }
    // free all arrays
    for (i = 0; i < alphabet_size; i++) free((void *) pheromone_trails[i]);
    free((void *) pheromone_trails);
    free((void *) ants);
    return global_best;
}
