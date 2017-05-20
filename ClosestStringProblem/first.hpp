//
//  first.hpp
//  ClosestStringProblem
//
//  Created by Arno Moonens on 9/05/17.
//  Copyright © 2017 Arno Moonens. All rights reserved.
//

#ifndef first_hpp
#define first_hpp

#include <stdio.h>
#include <math.h>

#include "instance.hpp"
#include "aco.hpp"
#include "solution.hpp"
#include "utils.hpp"

typedef Solution Ant;

class First : public ACO {
    
    void construct(Ant *current_ant);
    void local_search(Ant * ant);
    void local_search2(Ant * ant);
    double heuristic_information(Ant *current_ant, long int idx, long int char_idx);
    void update_pheromone_trails(Ant *global_best, double tau_min, double tau_max);

public:
    First(double pbeta, double prho, long int pseed);
    ~First();
    Solution * execute(Instance *inst, bool (*termination_criterion)(Solution *), void (*notify_improvement)(Solution *), long int nants);
};



#endif /* first_hpp */
