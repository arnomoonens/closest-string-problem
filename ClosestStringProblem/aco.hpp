//
//  aco.hpp
//  ClosestStringProblem
//
//  Created by Arno Moonens on 20/05/17.
//  Copyright © 2017 Arno Moonens. All rights reserved.
//

#ifndef aco_hpp
#define aco_hpp

#include <stdio.h>
#include <math.h>

#include "instance.hpp"
#include "solution.hpp"

typedef Solution Ant;

class ACO {
    
protected:
    long int seed;
    double beta;
    double rho;
    Instance * inst;
    double **pheromone_trails;
    double ** probability;
    
    virtual void construct(Ant *current_ant) = 0;
    double heuristic_information(long int idx, long int char_idx) {
         /** Number of strings that have character x at idx **/
        return (double) inst->getStringsPerCharCount()[idx][char_idx];
    };
    void initialize_pheromone_trails(double tau_init);
    void calculate_probability();
    void global_pheromone_update(Ant *global_best, double tau_min, double tau_max);

public:
    ACO(double pbeta, double prho, long int pseed);
    ~ACO();
    virtual Solution * execute(Instance *inst, bool (*termination_criterion)(Solution *), void (*notify_improvement)(Solution *), long int nants) = 0;
};

#endif /* aco_hpp */
