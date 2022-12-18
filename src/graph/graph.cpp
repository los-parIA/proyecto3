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
    size_t vertices = 0, edges = 0, colors = 0;
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

                switch (ch)
                {
                case 'c':
                    break;
                case 'p':
                    if (vertices || edges)
                        return g;
                    if (iss >> colors >> vertices >> edges)
                    {

                        g.V = vertices;
                        g.E = edges;
                        g.colors = colors;
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

int writeGraphToSatFile(graph g, string fileName)
{
    try
    {
        ofstream file;

        file.open(fileName, fstream::out);

        file << "p cnf " << g.colors * g.V << " " << g.V + g.V * g.colors * (g.colors - 1) / 2 + 2  * ( g.colors * g.E) << endl;

        int var[g.V + 1][g.colors + 1] ;
        int counter = 1, x = 1;
        while (x <= g.V)
        {
            int y = 1;
            while (y <= g.colors)
            {

                var[x][y] = counter;
                counter += 1;

                y += 1;
            }

            x += 1;
        }

        x = 1;
        while (x <= g.V)
        {
            int y = 1;
            while (y <= g.colors)
            {
                file << var[x][y] << " ";

                if (y == g.colors)
                    file << "0" << endl;

                y += 1;
            }

            x += 1;
        }

        x = 1;
        while (x <= g.V)
        {
            int y = 1;
            while (y <= g.colors - 1)
            {
                int z = y + 1;
                while (z <= g.colors)
                {
                    file << -var[x][y] << " " << -var[x][z] << " 0" << endl;
                    z += 1;
                }

                y += 1;
            }

            x += 1;
        }

        for (auto const &vu : g.G)
        {
            int v = vu.first;
            for (int u : vu.second)
            {
                int y = 1;
                while (y <= g.colors)
                {
                    file << -var[v][y] << " " << -var[u][y] << " 0" << endl;
                    y += 1;
                }
            }
        }

        file.close();

        return 0;
    }
    catch (...)
    {
        cerr << "Error: something went wrong while writing graph to cnf file" << endl;
        return 1;
    }
}
