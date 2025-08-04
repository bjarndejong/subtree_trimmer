#include <string>
#include <iostream>

#include "../src/graph.h"
#include "../src/treedecomp.h"
#include "../src/rootedtree.h"
#include "../src/postprocessor.h"
#include "../src/general.h"
#include "../src/binaryinteger.h"


using namespace std;

int main(int argc, char* argv[])
{
    // Input
    string filename_graph;
    filename_graph = argv[1];
    Graph G = Graph::from_file(filename_graph);

    //G.print();

    //size_t max_degree = 0;
    //for(int i = 0; i<G.N.size(); i++)
    //{
    //    max_degree = max(max_degree,G.N[i].size());
    //}
    //BinaryInteger::set_number_of_blocks(max_degree);

    //cout << max_degree << endl;


    string filename_td;
    filename_td = argv[2];
    TreeDecomp TD = TreeDecomp::from_file(filename_td);

    //TD.print();

    const int start = 1;
    RootedTree RT(TD.N,start);

    PostProcessor P(TD.bags,G);
    RootedTree RTCopy = RT;

    //RTCopy.print();

    vector<int> vertex_treated(G.N.size(),0);
    
    RT.df_traversal(
        [](RootedTree&){},
        [&vertex_treated,&TD,&P,&RTCopy](const int current, RootedTree&)
        {
            for(int i = 0; i<TD.bags[current-1].size(); i++)
            {
                if(vertex_treated[TD.bags[current-1][i]-1] == 0)
                {
                    //cout << "Bag " << current << " contained " << TD.bags[current-1][i] << endl;
                    vertex_treated[TD.bags[current-1][i]-1] = 1;
                    P.vertex_to_trim = TD.bags[current-1][i];
                    BinaryInteger::set_number_of_blocks(P.G.N[P.vertex_to_trim-1].size());
                    RTCopy.reroot(current);
                    RTCopy.df_traversal(
                    bind(&PostProcessor::setup, &P, placeholders::_1),//[](RootedTree&){cout << "setup" << endl;},//
                    bind(&PostProcessor::discover, &P, placeholders::_1, placeholders::_2),//[](const int, RootedTree&){cout << "discover" << endl;},//
                    bind(&PostProcessor::finish, &P, placeholders::_1, placeholders::_2),  //[](const int, RootedTree&){cout << "finish" << endl;},//
                    bind(&PostProcessor::cleanup, &P, placeholders::_1)//[](RootedTree&){}////[](RootedTree&){cout << "cleanup" << endl;}//
                    );
                    i--;
                }
            }
        },
        [](const int, RootedTree&){},
        [](RootedTree&){}
    );

    //for(int i = 0; i<G.N.size(); i++)
    //{
    //    P.vertex_to_trim = i+1;
    //    RTCopy.reroot(i+1);             //Set reroot to first occurence of i+1
    //    RTCopy.df_traversal(
    //        bind(&PostProcessor::setup, &P, placeholders::_1),
    //        bind(&PostProcessor::discover, &P, placeholders::_1, placeholders::_2),
    //        bind(&PostProcessor::finish, &P, placeholders::_1, placeholders::_2),  //[](const int, RootedTree&){} -- lambda dummy
    //        bind(&PostProcessor::cleanup, &P, placeholders::_1)//[](RootedTree&){}////
    //    );
    //}

    size_t max_bag_size = 0;
    for(size_t i = 0; i < TD.bags.size(); i++)
        max_bag_size = max(max_bag_size,TD.bags[i].size());

    ////////////////////////////////////////////////    Print .td format    //////////////////////////////////////////////////////////////////////////////////////
    cout << 's' << ' ' << "td" << ' ' << TD.bags.size() << ' ' << max_bag_size << ' ' << G.N.size() << endl;
    {
        for(size_t i = 0; i < TD.bags.size(); i++)
        {
            cout << 'b' << ' ' << i+1 << ' ';
            print_vector(TD.bags[i]);
        }
        for(int i = 0; i<TD.N.size(); i++)
        {
            for(int j = 0; j< TD.N[i].size(); j++)
            {
                if(i+1 < TD.N[i][j])
                    cout << i+1 << ' ' << TD.N[i][j] << endl;
            }
        }
    }
        
    return 0;
}