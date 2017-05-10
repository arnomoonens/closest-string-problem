//
//  aco.hpp
//  ClosestStringProblem
//
//  Created by Arno Moonens on 9/05/17.
//  Copyright © 2017 Arno Moonens. All rights reserved.
//

#ifndef aco_hpp
#define aco_hpp

#include <stdio.h>
#include <math.h>

#include "instance.hpp"
#include "solution.hpp"
#include "utils.hpp"

typedef Solution Ant;

class ACO {
    long int seed;
    double beta;
    double rho;
    Instance * inst;
    double **pheromone_trails;
    
    void construct(Ant *current_ant);
    void local_search(Ant * ant);
    void local_search2(Ant * ant);
    double heuristic_information(Ant *current_ant, int idx, char x);
    void update_pheromone_trails(Ant *global_best, double tau_min, double tau_max);

public:
    ACO(double pbeta, double prho, long int pseed);
    ~ACO();
    Solution * execute(Instance *inst, bool (*termination_criterion)(Solution *), void (*notify_improvement)(Solution *), long int nants);
};



#endif /* aco_hpp */
