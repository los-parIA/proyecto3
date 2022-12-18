#include <iostream>
#include <stdio.h>
#include <string.h>
#include <iomanip>
#include <chrono>
#include "graph/graph.hpp"
#include "satSolver/satSolver.hpp"

using namespace std::chrono;
using namespace std;

int main(int argc, char *argv[]) {	

   if( argc!=3 ){
      cout << "Numero erroneo de argumenos, el input es de la forma\n";
      cout << "./coloringSolver <archivo-grafo> <archivo-salida>\n";
      return 0;
   }

   // read the graph from file
   graph g = readGraphFromFile(argv[1]);
   string outputcnf = argv[1];   
   int l = 0, r = g.V+1, mid;
   while(l+1<r){
      mid = (l+r)/2;
      
      // valid k-coloring ?
      string satFile = outputcnf + ".cnf";
      g.colors = mid;
      writeGraphToSatFile(g, satFile);
      sat sa = readSatFromFile(satFile);
      solution sol = satSolver(sa,true);

      cout << "\n\n\n ?" << mid << " " << sol.foundSolution <<" \n\n\n";

      if( sol.foundSolution ) r = mid;
      else l = mid;
   }
   cout <<"La solucion es "<< r << endl;
   g.colors = r;


   // outup the solution to the k-coloring


   return 0;
}