//
//  acs.hpp
//  ClosestStringProblem
//
//  Created by Arno Moonens on 20/05/17.
//  Copyright © 2017 Arno Moonens. All rights reserved.
//

#ifndef second_hpp
#define second_hpp

#include <stdio.h>
#include <math.h>

#include "aco.hpp"

class ACS : public ACO {
    double exploitation_prob;
    double beta;
    bool use_local_search;
    double tau_init;
    
    void construct(Ant *current_ant);
    void local_search(Ant * ant);
    void initialize_pheromone_trails(double tau_init);
    void calculate_probability();
    void local_pheromone_update(Ant *global_best, long int idx, long int char_idx);
    void global_pheromone_update(Ant *global_best);
    
public:
    ACS(double pbeta, double prho, double pexploitation_prob, bool puse_local_search, long int pseed);
    ~ACS();
    Solution * execute(Instance *inst, bool (*termination_criterion)(Solution *), void (*notify_improvement)(Solution *), long int nants);
};

#endif /* acs_hpp */
