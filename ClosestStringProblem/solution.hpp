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
    long int * string_indices;
    long int * string_distances; // Distance of every instance string to the solution string
    long int string_max_dist_index;
    long int fx; // Solution quality = max(string_distances)
    Instance * inst;
    
public:
    Solution(Instance * pinst);
    ~Solution();
    void calculateSolutionQuality();
    void calculateSolutionQuality2();
    long int getSolutionQuality() {return fx;};
    void setSolutionQuality(long int sq) {fx = sq; return;};
    long int * getStringIndices() {return string_indices;};
    long int * getStringDistances() {return string_distances;};
    char * getString() {return string;};
    void setString(long int * indices);
    void setCharacter(long int idx, char ch); // Set using position in string and character
    void setCharacter(long int idx, long int char_idx); // set using position in string and character index in alphabet
};

#endif /* solution_hpp */
