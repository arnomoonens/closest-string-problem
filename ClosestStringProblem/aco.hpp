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
    
    virtual void construct(Ant *current_ant) = 0;
    virtual double heuristic_information(Ant *current_ant, long int idx, long int char_idx) = 0;
    virtual void update_pheromone_trails(Ant *global_best, double tau_min, double tau_max) = 0;

public:
    ACO(double pbeta, double prho, long int pseed);
    ~ACO();
    virtual Solution * execute(Instance *inst, bool (*termination_criterion)(Solution *), void (*notify_improvement)(Solution *), long int nants) = 0;
};

#endif /* aco_hpp */
