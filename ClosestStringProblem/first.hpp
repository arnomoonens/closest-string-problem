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
    
    bool use_local_search;
    
    void construct(Ant *current_ant);
    void local_search(Ant * ant);
    void local_search2(Ant * ant);
    void global_pheromone_update(Ant *global_best, double tau_min, double tau_max);

public:
    First(double pbeta, double prho, long int pseed, bool puse_local_search);
    ~First();
    Solution * execute(Instance *inst, bool (*termination_criterion)(Solution *), void (*notify_improvement)(Solution *), long int nants);
};



#endif /* first_hpp */
