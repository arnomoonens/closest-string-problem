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
    
    void construct(Ant *current_ant);
    void local_search(Ant * ant);
    void local_search2(Ant * ant);
    double heuristic_information(Ant *current_ant, long int idx, long int char_idx);
    void update_pheromone_trails(Ant *global_best, double tau_min, double tau_max);
    
public:
    ACS(double pbeta, double prho, double pexploitation_prob, long int pseed);
    ~ACS();
    Solution * execute(Instance *inst, bool (*termination_criterion)(Solution *), void (*notify_improvement)(Solution *), long int nants);
};

#endif /* acs_hpp */
