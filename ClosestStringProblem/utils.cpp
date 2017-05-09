//
//  utils.cpp
//  ClosestStringProblem
//
//  Created by Arno Moonens on 8/05/17.
//  Copyright © 2017 Arno Moonens. All rights reserved.
//

#include "utils.hpp"

/** print error when file can't be read **/
void error_reading_file(const char *text){
    printf("%s\n", text);
    exit( EXIT_FAILURE );
}


/* constants for a pseudo-random number generator, details see Numerical Recipes in C book */

#define IA 16807
#define IM 2147483647
#define AM (1.0/IM)
#define IQ 127773
#define IR 2836


double ran01( long *idum ) {
    /*
     FUNCTION:      returns a pseudo-random number
     INPUT:         a pointer to the seed variable
     OUTPUT:        a pseudo-random number uniformly distributed in [0,1]
     (SIDE)EFFECTS: changes the value of seed
     Notes:         call this function using ran01(&seed)
     */
    long k;
    double ans;
    
    k =(*idum)/IQ;
    *idum = IA * (*idum - k * IQ) - IR * k;
    if (*idum < 0 ) *idum += IM;
    ans = AM * (*idum);
    return ans;
}
