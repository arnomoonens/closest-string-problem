//
//  solution.hpp
//  ClosestStringProblem
//
//  Created by Arno Moonens on 8/05/17.
//  Copyright © 2017 Arno Moonens. All rights reserved.
//

#ifndef solution_hpp
#define solution_hpp

#include <stdio.h>

#include "instance.hpp"

class Solution {
    char * string;
    int * string_indices;
    int * string_distances; // Distance of every instance string to the solution string
    int fx; // Solution quality = max(string_distances)
    
public:
    Solution(Instance * inst);
    ~Solution();
    void calculateSolutionQuality(Instance * inst);
    int getSolutionQuality() {return fx;};
    int * getStringIndices() {return string_indices;};
    void setString(Instance * inst, int * indices);
};

#endif /* solution_hpp */
