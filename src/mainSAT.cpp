#include <iostream>
#include <stdio.h>
#include <string.h>
#include <iomanip>
#include <chrono>
#include "satSolver/satSolver.hpp"

using namespace std::chrono;
using namespace std;

int main(int argc, char *argv[]) {	

   if( argc!=2 && argc!=3 ){
      cout << "Numero erroneo de argumenos, el input es de la forma\n";
      cout << "./dpllSolver <archivo-cnf> -p\n";
      cout << "Where -p indicates pretty print (it's not mandatory)\n";
      return 0;
   }

   // read the file and save it in memory
   sat sa = readSatFromFile(argv[1]);   

   // start time
	auto start = high_resolution_clock::now();
   // solve the sat instance 
   solution sol = satSolver(sa);
   // end time
	auto stop = high_resolution_clock::now();
	
   // print the result
   string found = sol.foundSolution? "SATISFIABLE" : "UNSATISFIABLE";
   cout << found << '\n';
	auto duration = duration_cast<milliseconds>(stop - start);
	cout << fixed << setprecision(3) << "Tiempo: " << double(duration.count())/double(1000) << " s" << endl;

   if( argc>2 && strcmp(argv[2],"p") ){
      sa.variables = sol.variables;
      cout << satToStringPretty(sa) << '\n';
   }

   return 0;
}