#ifndef TREEDECOMP_H_INCLUDED
#define TREEDECOMP_H_INCLUDED

//INCLUDE STD
#include <vector>
#include <string>

class TreeDecomp
{
public:
    std::vector<std::vector<int>> bags;                   // bags[node - 1] are the bagcontents of node sorted in ascending order
    const std::vector<std::vector<int>> N;                      // N[node - 1] is the neighborhood of node


    static TreeDecomp from_file(const std::string& ifname);     // Static factory method
    //TreeDecomp(std::string ifname);
    void print() const;
    
private:
    TreeDecomp(std::vector<std::vector<int>>&& N, std::vector<std::vector<int>>&& bags);
};

#endif // TREEDECOMP_H_INCLUDED
