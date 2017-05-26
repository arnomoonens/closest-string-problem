#!/bin/sh

#  run.sh
#  ClosestStringProblem
#
#  Created by Arno Moonens on 11/05/17.
#  Copyright © 2017 Arno Moonens. All rights reserved.

#!/bin/sh

executable=$1
instances=$2
resultsfolder=$3
# $1 is path of csp executable
# $2 is path of folder with instances
# $3 is path to folder to save results

mkdir -p $resultsfolder

algos=(mmas acs)

for algo in "${algos[@]}"
do
    echo "Algorithm: $algo"
    destinationfile="$resultsfolder/$algo.txt"
    printf "" > $destinationfile
    for instancepath in "$instances"/* # Loop over every instance file in the instances folder
    do
        instance=$(echo $instancepath | sed "s/.*\/\(.*\)\.csp/\1/")
        printf "$instance" >> "$destinationfile"
        for run in {1..10}
        do
            cost=$(eval "$1 --seed $((run+10000)) --instance $instancepath --algo $algo --iterations 100 --beta 5.0 --rho 0.1")
            echo "Run $run on $instance: $cost"
            printf ",$cost" >> "$destinationfile"
        done
        printf "\n" >> "$destinationfile"
    done
    echo ""
done


# Local search
# Applying local search to the first one (MMAS)
algo=${algos[1]}
destinationfile="$resultsfolder/$algo-ls.txt"
printf "" > $destinationfile
for instancepath in "$instances"/* # Loop over every instance file in the instances folder
do
    instance=$(echo $instancepath | sed "s/.*\/\(.*\)\.csp/\1/")
    printf "$instance" >> "$destinationfile"
    for run in {1..10}
    do
        cost=$(eval "$1 --seed $((run+10000)) --instance $instancepath --algo $algo --iterations 100 --beta 5.0 --rho 0.1 --ls")
        echo "Run $run on $instance: $cost"
        printf ",$cost" >> "$destinationfile"
    done
    printf "\n" >> "$destinationfile"
done
