#include "postprocessor.h"

#include <vector>
#include <algorithm>
#include <iostream>

#include "rootedtree.h"
#include "binaryinteger.h"

using namespace std;

PostProcessor::PostProcessor(vector<vector<int>>& bags, const Graph& G)
    : bags(bags), G(G)
{

}

void PostProcessor::setup(RootedTree& RT)
{
    neighbourhoodmask.resize(RT.N.size());//c = vector<BinaryInteger>(RT.N.size(),BinaryInteger());
    is_set.resize(RT.N.size());
    degT.resize(RT.N.size());//keep = vector<int>(RT.N.size(),0);
    relevant_children.resize(RT.N.size());
}

void PostProcessor::discover(const int current, RootedTree& RT)
{
    int parent = RT.parents[current-1];
    neighbourhoodmask[current-1] = BinaryInteger();
    degT[current-1] = 0;
    is_set[current-1] = false;
    auto it = lower_bound(bags[current-1].begin(),bags[current-1].end(), vertex_to_trim);
    if(it != bags[current-1].end() && *it == vertex_to_trim)       
    {
        if(current != RT.root)
        {
            degT[current-1]=1;
            degT[parent-1]++;
        }
        //cout << current << " contains " << RT.root << endl;
        //int j = 0;
        //for(int i = 0; i<G.N[vertex_to_trim-1].size(); i++)
        //{
        //    while(j < bags[current-1].size() && bags[current-1][j] < G.N[vertex_to_trim-1][i])
        //        j++;
        //    if(j < bags[current-1].size() && bags[current-1][j] == G.N[vertex_to_trim-1][i])
        //        neighbourhoodmask[current-1].set_bit(i);//c[current-1] |= static_cast<__uint128_t>(1) << i;
        //} 
    }
    else
    {
        //cout << current << " did not contain " << RT.root << endl;
        RT.neighbourIterators[current-1] = RT.N[current-1].end();       // For df_traversal. Induces transition to finish
    }
}

void PostProcessor::finish(const int current, RootedTree& RT)
{
    int parent = RT.parents[current-1];
    //auto it = lower_bound(bags[current-1].begin(),bags[current-1].end(), vertex_to_trim);//
    //if(it != bags[current-1].end() && *it == vertex_to_trim)//(c[current-1] != 0)
    
    if(degT[current-1] == 1)
    {
        if(is_set[current-1] == false)
        {
            int j = 0;
            for(int i = 0; i<G.N[vertex_to_trim-1].size(); i++)
            {
                while(j < bags[current-1].size() && bags[current-1][j] < G.N[vertex_to_trim-1][i])
                    j++;
                if(j < bags[current-1].size() && bags[current-1][j] == G.N[vertex_to_trim-1][i])
                    neighbourhoodmask[current-1].set_bit(i);//c[current-1] |= static_cast<__uint128_t>(1) << i;
            }
            is_set[current-1] = true;
        }
        if(current != RT.root)
        {
            if(is_set[parent-1] == 0)
            {
                int j = 0;
                for(int i = 0; i<G.N[vertex_to_trim-1].size(); i++)
                {
                    while(j < bags[parent-1].size() && bags[parent-1][j] < G.N[vertex_to_trim-1][i])
                        j++;
                    if(j < bags[parent-1].size() && bags[parent-1][j] == G.N[vertex_to_trim-1][i])
                        neighbourhoodmask[parent-1].set_bit(i);//c[current-1] |= static_cast<__uint128_t>(1) << i;
                }
                is_set[parent-1] = true;
            }
            if((neighbourhoodmask[parent-1] | neighbourhoodmask[current-1]) == neighbourhoodmask[parent-1])
            {
                degT[current-1]=0;
                degT[parent-1]--;
                bags[current-1].erase(lower_bound(bags[current-1].begin(),bags[current-1].end(),vertex_to_trim));
            }
            else
                relevant_children[parent-1]=current;
        }
        //if(degT[current-1] == 1 && current != RT.root && ((neighbourhoodmask[parent-1] | neighbourhoodmask[current-1]) == neighbourhoodmask[parent-1]))//c[current-1].is_subset_of(c[parent-1]))//
        //{
            //cout << "Can remove " << RT.root << " from bag " << current << endl;
            //auto it = lower_bound(bags[current-1].begin(), bags[current-1].end(), RT.root);
            //if(it != bags[current-1].end() && *it == RT.root)
        //    bags[current-1].erase(it);
        //}
        //else    // Keep path from current till root for connectivity
        //{
        //    keep[current-1] = 1;
        //    keep[parent-1] = 1;
        //}
    }
    else
    {
        if(current!=RT.root && degT[current-1]>0)   //Exclude boundary nodes
            relevant_children[parent-1] = current;
    }
}

void PostProcessor::cleanup(RootedTree& RT)
{
    int current = RT.root;
    bool keep_reducing = (degT[current-1]==1);
    while(keep_reducing)
    {
        //int relevant_child = 0;
        //int number_of_relevant_children = 0;
        //bool stop = false;
        //for(int i = 0; !stop && i<RT.N[current-1].size(); i++)
        //{
        //    if(degT[RT.N[current-1][i]-1]>0) // && RT.parents[current-1] != RT.N[current-1][i])
        //    {
        int relevant_child = relevant_children[current-1];   //RT.N[current-1][i];
        if(is_set[relevant_child-1] == false)
        {
            int j = 0;
            for(int i = 0; i<G.N[vertex_to_trim-1].size(); i++)
            {
                while(j < bags[relevant_child-1].size() && bags[relevant_child-1][j] < G.N[vertex_to_trim-1][i])
                    j++;
                if(j < bags[relevant_child-1].size() && bags[relevant_child-1][j] == G.N[vertex_to_trim-1][i])
                    neighbourhoodmask[relevant_child-1].set_bit(i);//c[current-1] |= static_cast<__uint128_t>(1) << i;
            }
            is_set[relevant_child-1] = true;
        }
        if((neighbourhoodmask[current-1] | neighbourhoodmask[relevant_child-1]) == neighbourhoodmask[relevant_child-1])
        {
            bags[current-1].erase(lower_bound(bags[current-1].begin(),bags[current-1].end(),vertex_to_trim));
            degT[current-1] = 0;
            degT[relevant_child-1]--;
            keep_reducing = (degT[relevant_child-1]==1);
            current = relevant_child;
        }
        else
        {
            keep_reducing = false;
        }

            //    stop = true;
                //number_of_relevant_children++;
            //}
        //}
        //if(number_of_relevant_children == 1)
        //{
        //    if((neighbourhoodmask[current-1] | neighbourhoodmask[relevant_child-1]) == neighbourhoodmask[relevant_child-1])//(c[current-1].is_subset_of(c[relevant_child-1]))
        //    {
        //        auto it = lower_bound(bags[current-1].begin(), bags[current-1].end(), vertex_to_trim);
        //        if(it != bags[current-1].end() && *it == vertex_to_trim)
        //            bags[current-1].erase(it);
    //        }
    //        else
    //            keep_reducing = false;
    //    }
    //    else
    //        keep_reducing = false;
    //    current = relevant_child;
    //}
    }
}
