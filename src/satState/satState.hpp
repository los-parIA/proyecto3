#define FOR(i, n, m) for (int i = n; i < (int)m; i++)
#include <iostream>
#include <vector>
#include <fstream>
#include <set>
#include <string>
#include <istream>
#include <sstream>
#include <map>

using namespace std;

typedef long long ll;

const int NO_VALUE = 2;

struct sat
{
  // list of variables indexed by 0
  vector<int> variables;
  // list of clauses the variables are indexed from 1
  vector<vector<ll>> clauses;
  // remaining clauses to be checked
  set<int> clausesToCheck;
};

struct graph
{
  int V;
  int E;
  map<int, vector<int>> G;
};

string satToString(const sat &sa);
string satToStringPretty(const sat &sa);
string solSatToString(const sat &sa);
sat readSatFromFile(string fileName);
void saveSatInFile(const sat &sa, string path);