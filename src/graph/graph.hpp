#include <iostream>
#include <vector>
#include <fstream>
#include <set>
#include <string>
#include <istream>
#include <sstream>
#include <map>

using namespace std;

struct graph
{
  int V;
  int E;
  int colors;
  map<int, vector<int>> G;
};

string graphToString(const graph &g);

graph readGraphFromFile(string fileName);
int writeGraphToSatFile(graph g, string fileName);
