// this file have a copy of some functions of sarSolver
// to make this module 100% independent
#include "satSolverGA.hpp"

// =======================
//   Parsers to String  //
// =======================

string solutionToString(solution sol){
	string ans = "";
	string found = sol.foundSolution ? "SATISFIABLE" : "UNSATISFIABLE";
	ans += "we found a solution ? " + found;
	ans += "\n\n";
	if(sol.foundSolution){
		FOR(i, 0, sol.variables.size()){
			ans += "Var " + to_string(i) + " have value " + to_string(sol.variables[i]) + "\n";
		}
	}
	ans += "\n";
	return ans;
}

// ================= //
// Logic inference   //
// ================= //

bool inference(sat &sa, bool printE = false){
	bool cambio = true;
	while (cambio){
		cambio = false;
		vector<int> &variables = sa.variables;
		int n = sa.variables.size();

		// clauses to be erased
		vector<int> toErase;
		// variables to be set (can only have 1 value)
		vector<int> toSet;

		map<int, vector<pair<int, int>>> clausesOfPosition;

		// indexed from 0
		FOR(pos,0,sa.clauses.size()){
			auto c = sa.clauses[pos];
			bool validClause = false;
			int canBeTrue = 0, posVar = -1;
			// indexed from 1
			for (ll num : c){
				// remember clause
				if(variables[abs(num) - 1] == NO_VALUE)
					clausesOfPosition[abs(num)].push_back({pos, num});

				// if(p1 v p2 v p3) variable have to be true
				if(num > 0 && (variables[num - 1] == 1)){
					validClause = true;
				}
				// if(not p1 v not 2) varaible have to be false
				if(num < 0 && (variables[-num - 1] == 0)){
					validClause = true;
				}

				if(variables[abs(num) - 1] == NO_VALUE){					
					canBeTrue++;
					posVar = num;					
				}
			}

			if(!(validClause || canBeTrue))
				return false;
			if(validClause)
				toErase.push_back(pos);
			if(canBeTrue == 1 && !validClause){
				toSet.push_back(posVar);
				cambio = true;
			}
		}

		for (auto e : clausesOfPosition){
			vector<pair<int, int>> &posOfNum = e.second;

			if(posOfNum.size() == 1)
				continue;

			bool allEq = true;
			FOR(i, 1, posOfNum.size())
			allEq = allEq && bool(posOfNum[i - 1].second == posOfNum[i].second);

			if(allEq){
				for (auto e : posOfNum)
					toErase.push_back(e.first);
				toSet.push_back(e.first);
				cambio = true;
			}
		}

		// erase the clauses and set the variables for the inference
		for (int pos : toErase)
			if(sa.clausesToCheck.count(pos))
				sa.clausesToCheck.erase(sa.clausesToCheck.find(pos));
		for (int posVar : toSet){
			int v = posVar > 0 ? 1 : 0;
			if( sa.variables[abs(posVar)-1]==v ) continue;
			if( sa.variables[abs(posVar) - 1]!=NO_VALUE ) return false;
			sa.variables[abs(posVar) - 1] = v;
		}			
	}

	return true;
}

// ========================= //
// auxiliar function for the //
// genetic algorigm          //
// ========================= //

const int MAX_NUMBER_OF_MUTATION = 10;
const int MAX_NUMBER_OF_ATTEMPTS = 1000;
const int MAX_VARIABLES_TO_BT = 14;
const int PROB_SURVIVE = 1;
const int PROB_MERGE = 50;

void mutation(sat &sa){
	vector<int> aPositions;
	FOR(i,0,sa.variables.size())
		if(sa.variables[i]==NO_VALUE)
			aPositions.push_back(i);

	int N = min( MAX_NUMBER_OF_MUTATION, int(aPositions.size()) );
	set<int> used;

	int numMut = 0, numAttemps = 0;
	while(numMut < N && numAttemps <  MAX_NUMBER_OF_ATTEMPTS){
		numAttemps++;
		int p = rand() % aPositions.size();
		p = aPositions[p];

		if(used.count(p)) continue;
		used.insert(p);

		// save state
		int prevValVariable = sa.variables[p];
		set<int> clausesToCheck = sa.clausesToCheck;


		// mutation
		sa.variables[p] = rand() % 2;

		if(inference(sa)) numMut++;
		else{
			// return to prev state
			sa.variables[p] = prevValVariable;
			sa.clausesToCheck = clausesToCheck;

			// another try
			sa.variables[p] = 1 - sa.variables[p];
			if(inference(sa)) numMut++;
			else{
				FOR(i,0,sa.variables.size()) sa.variables[i] = NO_VALUE;
				FOR(i,0,sa.clauses.size()) sa.clausesToCheck.insert(i);				
				break;
			}
		}		
	}
}

vector<sat> randomCandidates(int n, int nVariables, vector<vector<ll>> clauses){
	vector<sat> ans;
	FOR(i, 0, n){
		sat s = {{}, {}, {}};
		// no values
		s.variables = vector<int>(nVariables,NO_VALUE);
		// copy clauses
		s.clauses = clauses;
		// all the clauses have to be checked
		FOR(j, 0, s.clauses.size())
			s.clausesToCheck.insert(j);
		// make valid mutations		
		mutation(s);	

		if(inference(s)) ans.push_back(s);
		else i--;
	}
	return ans;
}

sat merge(sat s1, sat s2){
	int N = s1.variables.size();
	sat ans = {vector<int>(N, NO_VALUE), s1.clauses, {}};
	FOR(i, 0, s1.clauses.size())
		ans.clausesToCheck.insert(i);

	FOR(i, 0, N){
		if(s1.variables[i] == s2.variables[i])
			ans.variables[i] = s1.variables[i];		
	}
	mutation(ans);

	return ans;
}

// ========================== //
// sat solver and heuristic   //
// ========================== //

bool isSatSolution(sat sa){
	vector<int> variables = sa.variables;
	int n = sa.variables.size();

	// indexed from 0
	FOR(pos,0,sa.clauses.size()){
		auto c = sa.clauses[pos];
		bool validClause = false;
		// indexed from 1
		for (ll num : c){
			// if(p1 v p2 v p3) variable have to be true
			if(num > 0 && (variables[num - 1] == 1)){
				validClause = true;
				break;
			}
			// if(not p1 v not 2) varaible have to be false
			if(num < 0 && (variables[-num - 1] == 0)){
				validClause = true;
				break;
			}

			// int num2 = num > 0 ? num : -num;
			// if(variables[num2 - 1] == NO_VALUE)
			// 	return false;
		}
		if(!validClause)
			return false;
	}

	return true;
}

void BTSolver(sat &sa){
	FOR(i,0,sa.variables.size()){
		if( sa.variables[i]==NO_VALUE ){
			FOR(newValue,0,2){
				sat s2 = { {}, {}, {} };
				s2.variables = sa.variables;
				s2.clauses = sa.clauses;
				s2.clausesToCheck = sa.clausesToCheck;

				s2.variables[i] = newValue;
				if( inference(s2) ){
					BTSolver(s2);
					if( isSatSolution(s2) ){
						sa = s2;
						break;
					}
				}
			}
			break;
		}
	}
}

int h(sat &sa){	
	// if the number of variables with NO_VALUE
	// is less than the limit => make backtracking
	int countVariables = 0;
	FOR(i,0,sa.variables.size())
		countVariables += sa.variables[i]==NO_VALUE;

	if(countVariables<MAX_VARIABLES_TO_BT){
		FOR(i,0,sa.clauses.size()) sa.clausesToCheck.insert(i);		
		BTSolver(sa);
		if( isSatSolution(sa) ) return 0;
		else{
			int impossibleValue = 0;
			for(auto c: sa.clauses)
				impossibleValue += c.size();
			return impossibleValue;
		}
	}

	// variablesClauses - variablesCompletedClauses
	vector<int> variables = sa.variables;
	int n = sa.variables.size();

	// indexed from 0
	int ans = 0, totAns = 0;
	for (auto c : sa.clauses){
		totAns += c.size();
		bool validClause = false;
		// indexed from 1
		for (ll num : c){
			// if(p1 v p2 v p3) variable have to be true
			if(num > 0 && (variables[num - 1] == 1)){
				validClause = true;
				break;
			}
			// if(not p1 v not 2) varaible have to be false
			if(num < 0 && (variables[-num - 1] == 0)){
				validClause = true;
				break;
			}
		}
		if(validClause)
			ans += c.size();
	}

	if(!inference(sa)) totAns;
	return totAns - ans;
}

void deleteRepeatedCandidates( vector<sat> &best ){
	map<vector<int>,bool> ready;
	vector<sat> ans;
	for(auto e: best){
		if( !ready[e.variables] ){
			ready[e.variables] = true;
			ans.push_back(e);
		}
	}
	best = ans;
}

solution satSolver(sat sa, int epochs, int Nindv, bool prettyPrint){
	// seed for random numbers
	srand(1);

	// random generation
	vector<sat> best = randomCandidates(Nindv, sa.variables.size(), sa.clauses);
	deleteRepeatedCandidates(best);

	FOR(e, 0, epochs){
		// mutate the best candidates
		// vector<sat> mut;
		// for(auto c: best){
		// 	sat m = { c.variables, c.clauses, c.clausesToCheck };
		// 	mutation(m);
		// 	mut.push_back(m);
		// }
		// best.insert(best.end(), mut.begin(), mut.end());


		// generate new random candidates to have variety
		vector<sat> randomCand = randomCandidates(Nindv, sa.variables.size(), sa.clauses);
		best.insert(best.end(), randomCand.begin(), randomCand.end());


		// merge any two candidates
		vector<sat> bestMerge;
		FOR(i, 0, best.size()){
			FOR(j, i + 1, best.size()) if(rand()%100<PROB_MERGE){
				sat newSat = merge(best[i], best[j]);
				bestMerge.push_back(newSat);
			}
		}
		best.insert(best.end(), bestMerge.begin(), bestMerge.end());
		deleteRepeatedCandidates(best);

		// sort and select the best candidates
		vector<pair<int, int>> valuesBest;
		FOR(i, 0, best.size())
			valuesBest.push_back({h(best[i]), i});
		sort(valuesBest.begin(), valuesBest.end());
		vector<sat> newBest;		
		FOR(i, 0, valuesBest.size()){
			if(i<Nindv){
				newBest.push_back(best[valuesBest[i].second]);		
			}else if( rand()%100<PROB_SURVIVE ){
				newBest.push_back(best[valuesBest[i].second]);		
			}
		}
		best = newBest;

		if(prettyPrint){
			cout << "\x1B[36mTras la generacion " << e + 1 << "\033[0m\t\t\n";
			cout <<"Sobrevivieron "<< best.size() << endl;
			int cnt = 0;
			for (auto e : best){
				if(++cnt==14) break;
				printf("\x1B[32mCandidato\033[0m\n");
				//cout << "Variables:" << satToStringPretty(e) << "\n";
				cout << "Heuristic value = " << h(e) << endl;
				cout << "\n";
			}
		}

		if( valuesBest[0].first==0 ) break;
	}

	// take the best one
	sat winner = best[0];
	FOR(i, 0, winner.variables.size())
	if(winner.variables[i] == NO_VALUE)
		winner.variables[i] = 0;

	bool isSolution = isSatSolution(winner);
	// complete not defined variables with random
	return {isSolution, winner.variables};
}