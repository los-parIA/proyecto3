# k-coloring solver (using SAT)
In this project we solve the k-coloring problem using two SAT solvers, one using only classic optimisations and the other one using genetic algorithms and the classic optimizations

## Classic SAT solver

We make a solver based on [DPLL](https://en.wikipedia.org/wiki/DPLL_algorithm) with optimisations:

### Pure literal elimination
### Unit propagation
### Bactracking

We decide to use a set (c++ data structure) the save the not checked clauses to have a better time response.

## Genetic algorithm

The genes are the values of the variables (0 or 1 or 2) of the SAT solution. In each iteration we add #numberOfindividuals random candidates, and merge them with the best candidates of the previous iteration. Then we apply an heuristic (clauses not solver) to sort the candidates and get the top (#numberOfindividuals candidates will survive) also we add a probability of 1% of survive for the rest of candidates. Each time we generate a candidate we make inferente with DPLL and we only make possible solutions. The merge function maintain the same genes and mutate the different genes.

## Binaries

You can make 3 binaries with the make command

* SAT solver (-p is optional)
>./dpllSolver <file-cnf> -p 

* SAT solver using genetic algorithm (-p is optional)
>./dpllSolver <file-cnf> <numberOfEpochs> <numberOfindividuals> -p

* k-coloring solver 
>./coloringSolver <k-coloring-file> <output-file-name>

(you can find input files in the test folder)
