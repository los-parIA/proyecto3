#include <iostream>
#include <stdio.h>
#include <string.h>
#include <iomanip>
#include <chrono>
#include "graph/graph.hpp"
#include "satSolver/satSolver.hpp"
//#include "satSolverGA/satSolverGA.hpp"

using namespace std::chrono;
using namespace std;

int main(int argc, char *argv[]) {	

   if( argc!=3 ){
      cout << "Numero erroneo de argumenos, el input es de la forma\n";
      cout << "./coloringSolver <archivo-grafo> <archivo-salida>\n";
      return 0;
   }

   // start time
	auto start = high_resolution_clock::now();

   // read the graph from file and make binary search
   // to find the solution of the problem
   graph g = readGraphFromFile(argv[1]);
   string outputcnf = argv[1];
   int l = 0, r = g.V+1, mid;
   vector<int> varSol;
   while(l+1<r){
      mid = (l+r)/2;

      // valid mid-coloring ?
      string satFile = outputcnf + ".cnf";
      g.colors = mid;
      writeGraphToSatFile(g, satFile);
      sat sa = readSatFromFile(satFile);
      solution sol = satSolver(sa,false);

      if( sol.foundSolution ) r = mid, varSol = sol.variables;
      else l = mid;
   }

   // end time
	auto stop = high_resolution_clock::now();

   // show solution and time
   cout << r << '\n';
   auto duration = duration_cast<milliseconds>(stop - start);
	cout << fixed << setprecision(3) << double(duration.count())/double(1000) << " s" << endl;

   // outup the solution as the k-coloring   
   saveKcolorinSolution( varSol, r, g.V, argv[2] );

   return 0;
}