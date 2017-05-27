# Closest string problem

## Usage

First, the C++ code needs to be compiled. This can be done in the terminal by executing (when being in the directory with the code):
```
g++ *.cpp CSP
```
his should result in a binary file called CSP. Some parameters can be passed to this file to influence the working of the program. These parameters are:
- `--algo <string>`: Algorithm to use. Can be `mmas` or `acs`. The default is `mmas`.
- `--ants <int>`: Number of ants to build every iteration. The default is 10.
- `--alpha <float>`: Alpha parameter (float). The default is 1.
- `--beta <float>`: Beta parameter (float). The default is 1.
- `--rho <float>`: Rho parameter (float). The default is 0.2.
- `--iterations <int>`: Maximum number of iterations to perform (interger). The default is 0 (disabled).
- `--seed <int>`: Number for the random seed generator.
- `--ls`: Use local search. If this parameter isn't given, local search is disabled.
- `--exploitation <float>`: Exploitation probability. The default is 0.9.
- `--instance <string>`: Path to the instance file

The analysis file, `analysis.R`, is an R script that can be called as such:
```
Rscript analysis.R
    /PATH/TO/RESULTS
    /PATH/TO/instances.opt
```
Some results of the analysis are printed and some are saved in the results directory that was passed to the program.
