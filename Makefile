CC = g++
CFLAGS = -I -g -w -Wall -std=c++14
BINARY_SAT_NAME = dpllSolver
BINARY_COLORING_NAME = coloringSolver

main: src/mainSAT.cpp src/satState/satState.cpp src/satSolver/satSolver.cpp
	$(CC) $(CFLAGS) -o $(BINARY_SAT_NAME)  src/mainSAT.cpp src/satState/satState.cpp src/satSolver/satSolver.cpp
