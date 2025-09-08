#include "graph.h"

//INCLUDE STD
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <functional>

using namespace std;

Graph::Graph(vector<vector<int>>&& N)
    : N(move(N)) {

}

Graph Graph::from_gr_file(const string& ifname)
{
    ifstream ifs{ifname};
    if (!ifs)
    {
        cerr << "Error: Failed to open .graph file '" << ifname << "'\n";
        exit(1); // or return, depending on your function
    }
    string input_holder{};
    //Process first line:               input line: p tw n m
    getline(ifs,input_holder);
    while(input_holder[0] == 'c')
        getline(ifs,input_holder);

    stringstream ss{input_holder};
    char p; string tw;
    int num_of_vertices;
    int num_of_edges;
    ss >> p >> tw >> num_of_vertices >> num_of_edges;

    vector<vector<int>> N(num_of_vertices);
    int edges_to_read = num_of_edges;
    int source;
    int target;
    while(edges_to_read--)
    {
        getline(ifs,input_holder);
        stringstream ss{input_holder};
        ss >> source >> target;
        N[source-1].push_back(target);
        N[target-1].push_back(source);
    }
    //for(int i = 0; i<N.size(); i++)
    //    sort(N[i].begin(),N[i].end());
    ifs.close();
    return Graph(move(N));
}

void Graph::print() const
{
    cout << "Graph:" << endl;
    for(vector<vector<int>>::const_iterator vertex_it = N.begin(); vertex_it != N.end(); vertex_it++)
    {
        cout << "N(" << vertex_it-N.begin()+1 << ") = {";
        for(vector<int>::const_iterator adj_it = vertex_it->begin(); adj_it != vertex_it->end(); adj_it++)
        {
            cout << *adj_it << " ";
        }
        cout << "}" << endl;
    }
}

bool Graph::independent_set(const vector<int>& subset) const
{
    for(vector<int>::const_iterator it1 = subset.begin(); it1!=subset.end(); it1++)
        for(vector<int>::const_iterator it2 = it1+1; it2 != subset.end(); it2++)
            if(adjacent(*it1,*it2))
                return false;
    return true;
}

bool Graph::adjacent(int u, int v) const
{
    return binary_search(N[u-1].begin(),N[u-1].end(),v);
}

int Graph::get_vertex_of_max_degree() const
{
    return 1 + distance
    (
        N.begin(),
        max_element(
            N.begin(), N.end(),
            [](const auto& a, const auto& b) {return a.size() < b.size();})
    );
}
