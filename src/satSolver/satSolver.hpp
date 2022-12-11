#define FOR(i,n,m) for(int i=n; i<m; i++)
#include <utility>
#include <vector>
#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <set>
#include "../satState/satState.hpp"

using namespace std;

const int INF = 0x3f3f3f3f;

struct solution{
    // bool to know if the algoeithm found a goal state or not
    bool foundSolution;    
    // vector with the variables
    vector<int> variables;
};

const solution nullSolution = {0,{}};
string solutionToString( solution s );
solution satSolver( sat sa );
