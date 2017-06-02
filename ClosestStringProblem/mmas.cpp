//
//  aco.cpp
//  ClosestStringProblem
//
//  Created by Arno Moonens on 9/05/17.
//  Copyright © 2017 Arno Moonens. All rights reserved.
//

#include "mmas.hpp"

MMAS::MMAS(Instance * pinst, double palpha, double pbeta, double prho, long int pseed, bool puse_local_search) : ACO(pinst, prho, pseed) {
    alpha = palpha;
    beta = pbeta;
    use_local_search = puse_local_search;
}

MMAS::~MMAS() {
}

/** Construction phase **/
void MMAS::construct(Ant *current_ant) {
    long int i, j, char_idx;
    double sum_prob, choice;
    long int string_length = inst->getStringLength();
    long int alphabet_size = inst->getAlphabetSize();
    double * selection_prob = (double *) malloc(alphabet_size * sizeof(double));
    for (i = 0; i < string_length; i++) { // For every position in the new string
        sum_prob = 0;
        for(j = 0; j < alphabet_size; j++) {
            sum_prob += probability[j][i];
            selection_prob[j] = sum_prob;
        }
        choice = ran01(&seed) * sum_prob;
        char_idx = 0;
        while (choice > selection_prob[char_idx]) char_idx++;
        current_ant->addCharacter(i, char_idx);
    }
    current_ant->computeSolutionQuality();
    free((void *) selection_prob);
    return;
}

void MMAS::initialize_pheromone_trails(double tau_init) {
    long int i, j;
    long int alphabet_size = inst->getAlphabetSize();
    long int string_length = inst->getStringLength();
    for (i = 0; i < alphabet_size; i++) {
        for (j = 0; j < string_length; j++) {
            pheromone_trails[i][j] = tau_init;
            probability[i][j] = pow(pheromone_trails[i][j], alpha) * pow(getHeuristicInformation(j, i), beta); // Update probabilities
        }
    }
}

void MMAS::calculate_probability() {
    long int i, j;
    long int alphabet_size = inst->getAlphabetSize();
    long int string_length = inst->getStringLength();
    for (i = 0; i < alphabet_size; i++) {
        for (j = 0; j < string_length; j++) {
            probability[i][j] = pow(pheromone_trails[i][j], alpha) * pow(getHeuristicInformation(j, i), beta);
        }
    }
}


/** Updating of pheromone trails of sets **/
void MMAS::global_pheromone_update(Ant *global_best, double tau_min, double tau_max) {
    long int j;
    long int string_length = inst->getStringLength();
    long int alphabet_size = inst->getAlphabetSize();
//    double delta_tau_max = (double) 1 - ((double) global_best->getSolutionQuality() / (double) string_length);
    double delta_tau_max = rho * tau_max; // rho * tau_max instead of the one above
    double delta_tau_min = rho * tau_min; // rho * tau_max instead of the one above
    double delta_tau;
    long int * string_indices = global_best->getStringIndices();
    double evaporation = 1.0 - rho;
    for (long int i = 0; i < string_length; i++) {
        for (j = 0; j < alphabet_size; j++) {
            delta_tau = (string_indices[i] == j) ? delta_tau_max : delta_tau_min;
            pheromone_trails[j][i] = (evaporation * pheromone_trails[j][i]) + delta_tau;
            if (pheromone_trails[j][i] < tau_min) {
                pheromone_trails[j][i] = tau_min;
            } else if (pheromone_trails[j][i] > tau_max) {
                pheromone_trails[j][i] = tau_max;
            }
            probability[j][i] = pow(pheromone_trails[j][i], alpha) * pow(getHeuristicInformation(i, j), beta); // Update probabilities
        }
    }
    return;
}

void MMAS::local_search(Ant * ant) {
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

//Local search v2
void MMAS::local_search2(Ant * ant) {
    long int str_dist, str_dist_n;
    long int * string_distances = ant->getStringDistances();
    long int n_strings = inst->getNumberOfStrings();
    for (long int i = 0; i < n_strings; i++) {
        str_dist = string_distances[i];
        str_dist_n = str_dist;
        
    }
}

/** Execute aco algorithm **/
Solution * MMAS::execute(bool (*termination_criterion)(Solution *), void (*notify_improvement)(Solution *), long int nants) {
    long int i;
    bool improvement;
    long int iterations_no_improvement = 0;
    Ant *global_best = NULL;
    Ant **ants = (Ant **) malloc(nants * sizeof(Ant *));
    long int alphabet_size = inst->getAlphabetSize();
    long int string_length = inst->getStringLength();
    double tau_max = (double) 1 / (double) alphabet_size;
    double tau_min = tau_max / ((double) alphabet_size * (double) string_length);
    compute_heuristic_information(inst);
    initialize_pheromone_trails(tau_max);
    while(!termination_criterion(global_best)) {
        improvement = false;
        for (i = 0; i < nants; i++) { // For each ant...
            ants[i] = new Ant(inst);
            construct(ants[i]); // Construct a solution...
            if (use_local_search) local_search(ants[i]); /// And apply local search if needed
            if (!global_best) {
                global_best = ants[i];
                improvement = true;
            } else if (ants[i]->getSolutionQuality() <= global_best->getSolutionQuality()) {
                if (ants[i]->getSolutionQuality() < global_best->getSolutionQuality()) {
                    improvement = true;
                }
                delete global_best;
                global_best = ants[i];
            } else {
                delete ants[i];
            }
        }
        if (improvement) { // New global best
            notify_improvement(global_best);
            iterations_no_improvement = 0;
//            tau_max = (double) 1 / (double) global_best->getSolutionQuality();
//            tau_min = tau_max / ((double) alphabet_size * (double) string_length);
        } else {
            iterations_no_improvement++;
            if(iterations_no_improvement % 100 == 0) {
#if DEBUG
                printf("REINIT\n");
#endif
                initialize_pheromone_trails(tau_max);
            }
        }
        global_pheromone_update(global_best, tau_min, tau_max);
    }
    free((void *) ants);
    return global_best;
}
