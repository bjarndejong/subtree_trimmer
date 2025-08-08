#include <string>
#include <iostream>

#include "../src/graph.h"
#include "../src/treedecomp.h"
#include "../src/rootedtree.h"
#include "../src/postprocessor.h"
#include "../src/general.h"
#include "../src/binaryinteger.h"
#include "../src/cli.h"


using namespace std;

int main(int argc, char* argv[])
{
    ////                                Input parameters                                ////
    string filename_graph;
    string filename_td;
    bool print_td = false;
        
    ////                                CLI dealings                                   ////
    vector<string> cliArguments;
    for(int i = 1; i<argc; i++)
        cliArguments.push_back(argv[i]);

    // Check for help
    int helpStatus = check_for_help(cliArguments);
    if(helpStatus < 2)
        return helpStatus;

    // Get td file, make sure exists and unique
    int tdStatus = check_for_td_file(cliArguments,filename_td);
    if(tdStatus==1)
        return tdStatus;

    // Get graph file, make sure exists and unique
    int graphStatus = check_for_graph_file(cliArguments,filename_graph);
    if(graphStatus==1)
        return graphStatus;

    //Check for option --print_td
    int optionStatus = check_for_options(cliArguments,print_td);
    if(optionStatus==1)
        return optionStatus;

    ////                               Set up according to parameters                  ////

    Graph G = Graph::from_file(filename_graph);
    TreeDecomp TD = TreeDecomp::from_file(filename_td);


    ////                                Begin actual program                            ////
    // Split TD info, which invalidates the TD object
    const int start = 1;
    RootedTree RT(move(TD.N),start);
    vector<vector<int>> bags = move(TD.bags);

    // Do the actual trimming
    PostProcessor P(bags,G);
    RootedTree RTCopy = RT;
    vector<int> vertex_treated(G.N.size(),0);
    
    RT.df_traversal(
        [](RootedTree&){},
        [&vertex_treated,&bags,&P,&RTCopy](const int current, RootedTree&)
        {
            for(int i = 0; i<bags[current-1].size(); i++)
            {
                if(vertex_treated[bags[current-1][i]-1] == 0)
                {
                    vertex_treated[bags[current-1][i]-1] = 1;
                    P.vertex_to_trim = bags[current-1][i];
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

    size_t max_bag_size = 0;
    for(size_t i = 0; i < bags.size(); i++)
        max_bag_size = max(max_bag_size,bags[i].size());

    ////////////////////////////////////////////////    Print .td format    //////////////////////////////////////////////////////////////////////////////////////
    cout << 's' << ' ' << "td" << ' ' << bags.size() << ' ' << max_bag_size << ' ' << G.N.size() << endl;
    if(print_td)
    {
        for(size_t i = 0; i < bags.size(); i++)
        {
            cout << 'b' << ' ' << i+1 << ' ';
            print_vector(bags[i]);
        }
        for(size_t i = 0; i<RT.N.size(); i++)
        {
            for(size_t j = 0; j< RT.N[i].size(); j++)
            {
                if(i+1 < RT.N[i][j])
                    cout << i+1 << ' ' << RT.N[i][j] << endl;
            }
        }
    }      
    return 0;
}