//
//  aco.cpp
//  ClosestStringProblem
//
//  Created by Arno Moonens on 20/05/17.
//  Copyright © 2017 Arno Moonens. All rights reserved.
//

#include "aco.hpp"


ACO::ACO(double pbeta, double prho, long int pseed) {
    beta = pbeta;
    rho = prho;
    seed = pseed;
}

ACO::~ACO() {
}
