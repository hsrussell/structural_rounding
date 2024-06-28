#ifndef solutionDS_hpp
#define solutionDS_hpp

#include <set>
#include <string>
#include <iostream>

class SolutionDS
{
  public:
    SolutionDS();
    std::string filename;
    int n; //nodes
    int k; //degeneracy
    int r; //target degeneracy
    int e; //edges
    double time_vertex_deletion = 0;
    double time_kernelization = 0;
    double time_lifting = 0;
    double time_DS_full_approx = 0;
    std::set<int> edit_set; //vertices in edit set
    int actual_k; //degeneracy after editing
    std::set<int> DS_partial_approx; //IS approx on editted graph
    std::set<int> DS_lifting_approx; //IS approx after lifting
    std::set<int> DS_full_approx; //IS approx on full graph
    void print();
  private:
    std::string set_to_csv(std::set<int> s);
};

#endif
