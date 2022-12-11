#include "satState.hpp"

// =======================
//   Parsers to String  //
// =======================

string satToString(const sat& sa ){
  string s 
    = "p cnf " 
    + to_string(sa.variables.size())
    + " " 
    + to_string(sa.clauses.size())
    + "\n";
  //for(auto variable : sa.variables)
  //  s += to_string(variable) + " ";
  //s += "0\n";

  for(auto clause : sa.clauses){
    for(auto variable : clause)
      s += to_string(variable) + " ";
    s += "0\n";
  }
  s.pop_back();
  s.pop_back();
  s += "0\n";

  return s;
}

string satToStringPretty(const sat& sa ){
  string ans = "";

  // variables
  for(auto e: sa.variables) ans += to_string(e) + " ";
  ans += "\n";

  // boolean expression
  for(auto c: sa.clauses){
		string clauseStr = "( ";
		for(auto var: c){
			int value;
      if(var>0) value = sa.variables[var-1];
      else value = 1 - sa.variables[-var-1];
			clauseStr += to_string(value) + " OR ";
		}
		FOR(i,0,3) clauseStr.pop_back();
		clauseStr += ") AND ";
		ans += clauseStr;
	}
  FOR(i,0,4) ans.pop_back();
  return ans;
}

string solSatToString(const sat& sa){
  string ans = to_string(sa.variables.size()) 
    + " "
    + to_string(sa.clauses.size());

    FOR(i,0,sa.variables.size()){
      int var = (sa.variables[i]?1:-1)*(i+1); 
      ans += "v "+to_string(var)+"\n";
    }

  return ans;
}

// =========================
//   functions for files  //
// =========================

string readFileIntoString(const string& path) {
    ifstream input_file(path);
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"
             << path << "'" << endl;
        exit(EXIT_FAILURE);
    }
    return string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
}

vector<string> split(string &s, char c){
    vector<string> ans;
    string temp = "";
    FOR(i,0,s.size()){
        if(s[i]==c) ans.push_back(temp), temp = "";
        else temp += s[i];
    }
    if(temp.size()) ans.push_back(temp);
    return ans;
}

sat readSatFromFile(string fileName){
    sat ans = { {}, {}, {} };
    string f = readFileIntoString(fileName);

    vector<string> lines = split(f,'\n');
    for(string line: lines){
        vector<string> lineBySpace = split(line,' ');
        if(lineBySpace.size()<1 ||lineBySpace[0]=="c" ) 
            continue;

        if( lineBySpace.size()>3 && lineBySpace[0]=="p" && lineBySpace[1]=="cnf" ){
            int n = stoi(lineBySpace[2]);
            ans.variables = vector<int>(n,NO_VALUE);
        }else{
            vector<ll> c;
            for(auto s: lineBySpace) c.push_back(stoi(s));
            c.pop_back();
            ans.clauses.push_back(c);
        }
    }

    // all the clauses have to be checked
    FOR(i,0,ans.clauses.size()) 
        ans.clausesToCheck.insert(i);
    return ans;
}

void saveSatInFile(const sat& sa, string path){
  fstream file;
  file.open(path, ios_base::out);

  if(!file.is_open()){
      cout<<"Unable to open the file.\n";
      exit(1);
  }
  file<<satToString(sa);
  file.close(); 
}