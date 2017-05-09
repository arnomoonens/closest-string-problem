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
    double beta;
    double rho;
    double epsilon;
    Instance * inst;
    double **pheromone_trails;
    
    void construct(Ant *current_ant);
    
public:
    ACO(double beta, double rho, double epsilon);
    ~ACO();
    Solution * execute(Instance *inst, bool (*termination_criterion)(Solution *), void (*notify_improvement)(Solution *), long int nants);
    double heuristic_information(Ant *current_ant, int idx, char x);
    void update_pheromone_trails(Ant *global_best, double tau_min, double tau_max);
};



#endif /* aco_hpp */
