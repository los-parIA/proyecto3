CC = g++
CFLAGS = -I -g -w -Wall -std=c++14
BINARY_SAT_NAME = dpllSolver
BINARY_COLORING_NAME = coloringSolver

main: src/mainSAT.cpp src/satState/satState.cpp src/satSolver/satSolver.cpp
	$(CC) $(CFLAGS) -o $(BINARY_SAT_NAME)  src/mainSAT.cpp src/satState/satState.cpp src/satSolver/satSolver.cpp

mainSatGa: src/mainSATGA.cpp src/satState/satState.cpp src/satSolverGA/satSolverGA.cpp
	$(CC) $(CFLAGS) -o $(BINARY_SAT_NAME) src/mainSATGA.cpp src/satState/satState.cpp src/satSolverGA/satSolverGA.cpp

mainColoring: src/mainColoring.cpp src/graph/graph.cpp src/satState/satState.cpp src/satSolver/satSolver.cpp
	$(CC) $(CFLAGS) -o $(BINARY_COLORING_NAME) src/mainColoring.cpp src/graph/graph.cpp src/satState/satState.cpp src/satSolver/satSolver.cpp

clean:
	rm  -f $(BINARY_COLORING_NAME), $(BINARY_SAT_NAME)

