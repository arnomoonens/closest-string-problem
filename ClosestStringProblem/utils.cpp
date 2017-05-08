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
