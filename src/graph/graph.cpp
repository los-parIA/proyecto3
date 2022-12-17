#include "graph.hpp"

// =======================
//   Parsers to String  //
// =======================

string graphToString(const graph &g)
{
    string s = "#VERTICES: " + to_string(g.V) + " #EDGES: " + to_string(g.E) + "\n";
    for (auto const &x : g.G)
    {
        s += "\nLista de adyacencia de " + to_string(x.first) + "\n head";

        for (int y : x.second)
            s += " -> " + to_string(y);
        s += "\n";
    }

    return s;
}

// =======================
//  Graph related funcs //
// =======================

void addEdge(graph &g, int from, int to)
{
    if (g.G.count(from))
    {
        g.G[from].push_back(to);
    }
    else
    {
        g.G.insert({from, {to}});
    }

    if (g.G.count(to))
    {
        g.G[to].push_back(from);
    }
    else
    {
        g.G.insert({to, {from}});
    }
}

graph readGraphFromFile(string fileName)
{
    size_t vertices = 0, edges = 0;
    graph g;

    try
    {
        ifstream file;
        file.open(fileName);

        if (!file)
        {
            cerr << "Error: file couldn't be opened" << endl;
            return g;
        }

        std::string line;
        while (getline(file, line))

        {
            istringstream iss(line);
            char ch;
            if (iss >> ch)
            {
                size_t from, to;
                string format;

                switch (ch)
                {
                case 'c':
                    break;
                case 'p':
                    if (vertices || edges)
                        return g;
                    if (iss >> format >> vertices >> edges)
                    {
                        if ("edge" != format)
                            return g;

                        g.V = vertices;
                        g.E = edges;
                    }
                    break;
                case 'e':
                    if (edges-- && (iss >> from >> to))
                    {
                        addEdge(g, from, to);
                    }
                    break;
                default:
                    return g;
                }
            }
        }

        return g;
    }
    catch (...)
    {
        cerr << "Error: something went wrong while reading file" << endl;
        return g;
    }
}
