//
//  main.cpp
//  ClosestStringProblem
//
//  Created by Arno Moonens on 8/05/17.
//  Copyright © 2017 Arno Moonens. All rights reserved.
//

#include <iostream>

#include "instance.hpp"
#include "aco.hpp"
#include "first.hpp"
#include "acs.hpp"
#include "solution.hpp"

char * instance_file=NULL;
long int max_iterations;   //Max iterations
double alpha;
double beta;
double rho;
long int n_ants;
long int seed = -1;
double initial_pheromone=1.0;
long int iterations = 0;
bool use_local_search;
double exploitation_prob;
char * algorithm;

void printHelp(){
    std::cout << "\nACO Usage:\n"
    << "   ./aco [--algo <first|acs>] [--ants <int>] [--alpha <float>] [--beta <float>] [--rho <float>] [--iterations <int>] [--seed <int>] [--ls] [--exploitation <float>] --instance <path>\n\n"
    << "Example: ./aco --tours 2000 --seed 123 --instance eil151.tsp\n\n"
    << "\nACO flags:\n"
    << "   --algo: Algorithm to use. Can be \"first\" or \"acs\". Default=\"first\".\n"
    << "   --ants: Number of ants to build every iteration. Default=10.\n"
    << "   --alpha: Alpha parameter (float). Default=1.\n"
    << "   --beta: Beta parameter (float). Default=1.\n"
    << "   --rho: Rho parameter (float). Defaut=0.2.\n"
    << "   --iterations: Maximum number of iterations to perform (interger). Default:0 (disabled).\n"
    << "   --seed: Number for the random seed generator.\n"
    << "   --ls: Use local search.\n"
    << "   --exploitation: Exploitation probability.\n"
    << "   --instance: Path to the instance file\n"
    << "\nACO other parameters:\n"
    << "   initial pheromone: " << initial_pheromone << ".\n"
    << std::endl;
}

/*Default parameters: set them!*/
void setDefaultParameters(){
    alpha=1.0;
    beta=1.0;
    rho=0.2;
    n_ants=10;
    max_iterations=500;
    instance_file=NULL;
    seed=seed = (long int) time(NULL);
    use_local_search = false;
    exploitation_prob = 0.9;
    algorithm = (char *) "first";
}

/*Print default parameters*/
void printParameters(){
    std::cout << "\nACO parameters:\n"
    << "  ants: "  << n_ants << "\n"
    << "  alpha: " << alpha << "\n"
    << "  beta: "  << beta << "\n"
    << "  rho: "   << rho << "\n"
    << "  iterations: "   << max_iterations << "\n"
    << "  seed: "   << seed << "\n"
    << "  initial pheromone: "   << initial_pheromone << "\n"
    << "  use local search: " << use_local_search << "\n"
    << std::endl;
}

/** Used by ils and aco to determine when to stop **/
bool termination_criterion(Solution *sol) {
    return ++iterations > max_iterations;
}

/** Callback when better solution is encountered: write time and quality to file **/
void notify_improvement(Solution * sol) {
#if DEBUG
    printf("%li %li\n", iterations, sol->getSolutionQuality());
#endif
    return;
}

/* Read arguments from command line */
bool readArguments(int argc, char *argv[] ){
    
    setDefaultParameters();
    
    for(int i=1; i< argc ; i++){
        if(strcmp(argv[i], "--ants") == 0){
            n_ants = atol(argv[i+1]);
            i++;
        } else if(strcmp(argv[i], "--alpha") == 0){
            alpha = atof(argv[i+1]);
            i++;
        } else if(strcmp(argv[i], "--beta") == 0){
            beta = atof(argv[i+1]);
            i++;
        } else if(strcmp(argv[i], "--rho") == 0) {
            rho = atof(argv[i+1]);
            i++;
        } else if(strcmp(argv[i], "--iterations") == 0) {
            max_iterations = atol(argv[i+1]);
            i++;
        } else if(strcmp(argv[i], "--seed") == 0) {
            seed = atol(argv[i+1]);
            i++;
        } else if(strcmp(argv[i], "--ls") == 0) {
            use_local_search = true;
        } else if(strcmp(argv[i], "--exploitation") == 0) {
            exploitation_prob = atof(argv[i+1]);
            i++;
        } else if(strcmp(argv[i], "--algo") == 0) {
            algorithm = argv[i+1];
            i++;
        } else if(strcmp(argv[i], "--instance") == 0) {
            instance_file = argv[i+1];
            i++;
        } else if(strcmp(argv[i], "--help") == 0) {
            printHelp();
            return(false);
        } else {
            std::cout << "Parameter \"" << argv[i] << "\" not recognized." << std::endl;
            return(false);
        }
    }
    if(instance_file == NULL){
        std::cout << "No instance file provided.\n";
        return(false);
    }
//    printParameters();
    return(true);
}

int main(int argc, char *argv[] ){
    if(!readArguments(argc, argv)){
        exit(1);
    }
    Instance * inst = new Instance(instance_file);
//    inst->print();
    ACO * algo;
    if (strcmp(algorithm, "first") == 0) {
        algo = new First(beta, rho, seed, use_local_search);
    } else {
        algo = new ACS(beta, rho, exploitation_prob, use_local_search, seed);
    }
    Solution * sol = algo->execute(inst, termination_criterion, notify_improvement, n_ants);
    printf("%li", sol->getSolutionQuality());
    delete sol;
    delete inst;
    return 0;
}
