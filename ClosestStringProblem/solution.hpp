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
    int string_max_dist_index;
    int fx; // Solution quality = max(string_distances)
    Instance * inst;
    
public:
    Solution(Instance * pinst);
    ~Solution();
    int calculateSolutionQuality();
    int calculateSolutionQuality2();
    int getSolutionQuality() {return fx;};
    void setSolutionQuality(int sq) {fx = sq; return;};
    int * getStringIndices() {return string_indices;};
    int * getStringDistances() {return string_distances;};
    char * getString() {return string;};
    void setString(int * indices);
    void setCharacter(int idx, char ch); // Set using position in string and character
    void setCharacter(int idx, int char_idx); // set using position in string and character index in alphabet
};

#endif /* solution_hpp */
