#include <iostream>
#include <stdio.h>
#include <string.h>
#include <iomanip>
#include <time.h>
#include <chrono>
#include "satSolverGA/satSolverGA.hpp"

using namespace std::chrono;
using namespace std;

int main(int argc, char *argv[]) {	

   if( argc!=4 && argc!=5 ){
      cout << "Numero erroneo de argumenos, el input es de la forma\n";
      cout << "./dpllSolver <archivo-cnf> <numberOfEpochs> <numberOfindividuals> -p\n\n";
      cout << "Donde <numberOfEpochs> es el numero de epocas del algoritmo genetico\n";
      cout << "Donde <numberOfindividuals> es el numero de individuos por epoca del algoritmo genetico\n";
      cout << "Donde -p indice una output con un formato mas bonito\n";
      return 0;
   }

	// read the parameters
  int epochs = stoi(argv[2]);
  int Nindv = stoi(argv[3]);
	bool prettyPrint = argc>4 && strcmp(argv[4],"p");
	// read the file and save it in memory 
	sat sa = readSatFromFile(argv[1]);   

	// start time
	auto start = high_resolution_clock::now();
	// solve the sat instance 
	solution sol = satSolver(sa,epochs,Nindv,prettyPrint);
	// end time
	auto stop = high_resolution_clock::now();

	// print the result
	string found = sol.foundSolution? "SATISFIABLE" : "UNSATISFIABLE";
	cout << found << '\n';
	auto duration = duration_cast<milliseconds>(stop - start);
	cout << fixed << setprecision(3) << "Tiempo: " << double(duration.count())/double(1000) << " s" << endl;

	if( prettyPrint && sol.foundSolution ){
		// value 2 represent no assigned variables
		sa.variables = sol.variables;
		cout << satToStringPretty(sa) << '\n';
	}

   return 0;
}