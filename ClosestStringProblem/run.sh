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
    mkdir -p "$resultsfolder/$algo"
    for instancepath in "$instances"/* # Loop over every instance file in the instances folder
    do
        instance=$(echo $instancepath | sed "s/.*\/\(.*\)\.csp/\1/")
        printf "$instance" >> "$destinationfile"
        for run in {1..10}
        do
            result=$(eval "$executable --seed $((run+10000)) --instance $instancepath --algo $algo --iterations 1000 --beta 5.0 --rho 0.1")
            convergence=$(echo $result | sed \$d)
            echo $convergence > "$resultsfolder/$algo/$instance-$run.txt"
            cost=$(echo $result | tail -n 1)
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
mkdir -p "$resultsfolder/$algo-ls"
for instancepath in "$instances"/* # Loop over every instance file in the instances folder
do
    instance=$(echo $instancepath | sed "s/.*\/\(.*\)\.csp/\1/")
    printf "$instance" >> "$destinationfile"
    for run in {1..10}
    do
        result=$(eval "$1 --seed $((run+10000)) --instance $instancepath --algo $algo --iterations 1000 --beta 5.0 --rho 0.1 --ls")
        convergence=$(echo $result | sed \$d)
        echo $convergence > "$resultsfolder/$algo-ls/$instance-$run.txt"
        cost=$(echo $result | tail -n 1)
        echo "Run $run on $instance: $cost"
        printf ",$cost" >> "$destinationfile"
    done
    printf "\n" >> "$destinationfile"
done
