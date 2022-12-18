#include "satSolver.hpp"

// =======================
//   Parsers to String  //
// =======================

string solutionToString( solution sol ){
	string ans = "";
    string found = sol.foundSolution? "SATISFIABLE" : "UNSATISFIABLE";
	ans += "we found a solution ? " + found;
    ans += "\n\n";
    if( sol.foundSolution ){
        FOR(i,0,sol.variables.size()){
            ans += "Var "+ to_string(i) + " have value "+ to_string(sol.variables[i]) + "\n";
        }
    }
    ans += "\n";
	return ans;
}

// ========================= //
//   Auxiliar functions      //
//   to optimize the next    //
//   states                  //
// ========================= //

bool isPossibleState( sat &sa ){
	bool cambio = true;
	while(cambio){
		cambio = false;
		vector<int> &variables = sa.variables;
		int n = sa.variables.size();

        // clauses to be erased
		vector<int> toErase;
        // variables to be set (can only have 1 value)
		vector<int> toSet;

		map<int,vector<pair<int,int>>> clausesOfPosition;

		// indexed from 0
		for(int pos: sa.clausesToCheck){
			auto c = sa.clauses[pos];
			bool validClause = false;
			int canBeTrue = 0, posVar = -1;
			// indexed from 1
			for(ll num: c){
				// remember clause
				if( variables[abs(num)-1]==NO_VALUE ) 
					clausesOfPosition[abs(num)].push_back({pos,num});

				// if (p1 v p2 v p3) variable have to be true
				if( num>0 && (variables[num-1]==1) ){
					validClause = true;
				}
				// if (not p1 v not 2) varaible have to be false
				if( num<0 && (variables[-num-1]==0) ){
					validClause = true;
				}

				if(variables[abs(num)-1]==NO_VALUE){ 
					canBeTrue++;
					posVar = num;
				}
			}

			if(!(validClause||canBeTrue)) return false;
			if(validClause) toErase.push_back(pos);
			if( canBeTrue == 1 && !validClause ){				
				toSet.push_back(posVar);
				cambio=true;
			}
		}

		for(auto e: clausesOfPosition){
			vector<pair<int,int>> &posOfNum = e.second;

			if(posOfNum.size()==1) continue;

			bool allEq = true;
			FOR(i,1,posOfNum.size())
				allEq = allEq && bool(posOfNum[i-1].second==posOfNum[i].second);
			
			if(allEq){
				for(auto e: posOfNum) toErase.push_back(e.first);
				toSet.push_back(e.first);
				cambio = true;
			}
		}

        // erase the clauses and set the variables for the inference
        for(int pos : toErase)
            if( sa.clausesToCheck.count(pos) ) 
                sa.clausesToCheck.erase(sa.clausesToCheck.find(pos));
        for(int posVar: toSet)
            sa.variables[abs(posVar)-1] = posVar>0?1:0;
	}

	return true;
}

vector<sat> nextStates( sat sa ){
	vector<sat> ans;
	vector<int> variables = sa.variables;
	int n = variables.size();


	FOR(i,0,n) if(variables[i]==NO_VALUE){
		vector<int> newVariablesTrue = variables;
		newVariablesTrue[i] = 1;
		sat stateTrue = { newVariablesTrue, sa.clauses, sa.clausesToCheck };
		if(isPossibleState(stateTrue)){
			ans.push_back(stateTrue);
		}

		vector<int> newVariablesFalse = variables;
		newVariablesFalse[i] = 0;
		sat stateFalse = { newVariablesFalse, sa.clauses, sa.clausesToCheck };
		if(isPossibleState(stateFalse)){
			ans.push_back(stateFalse);
		} 
		break;
	}	

	return ans;
}

// ========================== //
// sat solver and heuristic   //
// ========================== //

bool isSatSolution(sat &sa){
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

		// indexed from 0, we should use only sa.clausesToCheck 
		// to get the positions (pos) ...
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
			if( sa.variables[abs(posVar) - 1]!=NO_VALUE ) return false;
			sa.variables[abs(posVar) - 1] = posVar > 0 ? 1 : 0;
		}			
	}

	return true;
}

int h( sat sa ){
    // clausulas - clausulasCompletadas
    vector<int> variables = sa.variables;
	int n = sa.variables.size();

    // indexed from 0
    int ans = 0, totAns = 0;
	for(auto c: sa.clauses){
        totAns += c.size();
		bool validClause = false;
		// indexed from 1
		for(ll num: c){
			// if (p1 v p2 v p3) variable have to be true
			if( num>0 && (variables[num-1]==1) ){
				validClause = true;
				break;
			}
			// if (not p1 v not 2) varaible have to be false
			if( num<0 && (variables[-num-1]==0) ){
				validClause = true;
				break;
			}
		}
		if(validClause) ans+=c.size();
	}

    return totAns - ans;
}

solution satSolver( sat sa ){
    if( isSatSolution(sa) ) return {1,sa.variables};
    vector<sat> nextS = nextStates(sa);

    vector<pair<int,int>> vals;
    FOR(i,0,nextS.size()) vals.push_back({h(nextS[i]),i});
    sort(vals.begin(),vals.end());

    for(auto p: vals){
        solution ans = satSolver( nextS[p.second] );
        if(ans.foundSolution) return ans;
    }
    return {0,sa.variables};
}