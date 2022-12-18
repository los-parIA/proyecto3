#include <iostream>
#include <stdio.h>
#include <string.h>
#include <iomanip>
#include <chrono>
#include "graph/graph.hpp"

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

   cout << graphToString(g);

   string outputcnf = argv[1];
   int a = writeGraphToSatFile(g, outputcnf + ".cnf");

   return 0;
}