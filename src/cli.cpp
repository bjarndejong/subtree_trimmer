#include "cli.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Get graph file, make sure exists and unique
int check_for_graph_file(const vector<string>& cliArguments, string& filename_graph)
{
    int number_of_graph_files = 0;
    for(int i = 0; i<cliArguments.size(); i++)
    {
        if (cliArguments[i].size() >= 6 && cliArguments[i].substr(cliArguments[i].size() - 6) == ".graph")
        {
            number_of_graph_files++;
            filename_graph = cliArguments[i];
        }
    }
    if(number_of_graph_files != 1)
    {
        cout << "Arguments must contain a single .graph file" << endl;
        return 1;
    }
    else
        return 0;
}

// Get td file, make sure exists and unique
int check_for_td_file(const vector<string>& cliArguments, string& filename_td)
{
    int number_of_td_files = 0;
    for(int i = 0; i<cliArguments.size(); i++)
    {
        if (cliArguments[i].size() >= 3 && cliArguments[i].substr(cliArguments[i].size() - 3) == ".td")
        {
            number_of_td_files++;
            filename_td = cliArguments[i];
        }
    }
    if(number_of_td_files != 1)
    {
        cout << "Arguments must contain a single .td file" << endl;
        return 1;
    }
    else 
        return 0;
}

// Check for print_td option
int check_for_options(const vector<string>& cliArguments, bool& print_td)
{
    if(cliArguments.size()>=2 && cliArguments.size()<=3)
    {
        int number_of_print_options = 0;
        for(int i = 0; i<cliArguments.size(); i++)
        {
            if(cliArguments[i].size() >= 11 && cliArguments[i].substr(0, 11) == "--print_td=")
            {
                number_of_print_options++;
                if(cliArguments[i] == "--print_td=true" || cliArguments[i] == "--print_td=false")
                {
                    if(cliArguments[i] == "--print_td=true")
                        print_td=true;
                    else
                        print_td=false;
                }
                else
                {
                    cout << "Optional argument --print_td is set incorrectly" << endl;
                    return 1;
                }
            }
        }
        if(number_of_print_options>1)
        {
            cout << "Can only set --print_td once" << endl;
            return 1;
        }
    }
    else
    {
        cout << "Takes at most 3 arguments" << endl;
        return 1;
    }
    return 0;
}

//Check for help
int check_for_help(const vector<string>& cliArguments)
{
    bool help_was_requested = false;
    for(int i = 0; i<cliArguments.size(); i++)
    {
        if(cliArguments[i]=="--help")
            help_was_requested = true;
    }
    if(help_was_requested)
    {
        if(cliArguments.size()==1)
        {
            cout << "Usage:\n"
            << "  subtree-trimmer <tree_decomposition.td> <graph.graph> [options]\n\n"

            << "Description:\n"
            << "  subtree-trimmer removes vertices v from ends of v-subtrees, modifying bag contents,\n"
            << "  only under conditions that preserve the validity and tree structure of the decomposition.\n\n"
          
            << "Arguments:\n"
            << "  <tree_decomposition.td>     Path to the input tree decomposition file (.td).\n"
            << "                              Must follow the PACE 2017 format. Additionally, bags and bag contents must be sorted.\n"
            << "  <graph.graph>               Path to the input graph file (.graph)\n\n"
          
            << "Options:\n"
            << "  --print_td=true|false         Print the entire .td file if true\n"
            << "                                Default: false\n"
            << "  --help                        Show this help message and exit (must be used alone)\n\n"
          
            << "Examples:\n"
            << "  subtree-trimmer tdfile.td graphfile.graph\n"
            << "  subtree-trimmer graphfile.graph --print_td=true tdfile.td\n\n"

            << "Output:\n"
            << "  The program prints to standard output either:\n"
            << "    - the 's' line of the trimmed tree decomposition (default), or\n"
            << "    - the entire trimmed tree decomposition file (if --print_td=true).\n\n"

            << "Note:\n"
            << "  Input files are never modified.\n"
            << "  Comments from the original file are not included in the output.\n\n";

            return 0;
        }
        else
        {
            cout << "--help must be the only argument" << endl;
            return 1;
        } 
    }
    else
        return 2;
}