#!/bin/sh

#  run.sh
#  ClosestStringProblem
#
#  Created by Arno Moonens on 11/05/17.
#  Copyright © 2017 Arno Moonens. All rights reserved.

#!/bin/sh

# $1 is path of csp executable
# $2 is path of folder with instances
# $3 is path to folder to save results

mkdir -p "$3"

for instancepath in "$2"/* # Loop over every instance file in the instances folder
do
instance=$(echo $instancepath | sed "s/.*\/\(.*\)\.csp/\1/")
destinationfile="$3/$instance.txt"
printf "" > "$destinationfile"
for run in {1..10}
do
cost=$(eval "$1 --seed $((run+10000)) --instance $instancepath --iterations 100 --beta 5.0 --rho 0.1")
echo "Result for $instance: $cost"
echo "$cost" >> "$destinationfile"
done
done
