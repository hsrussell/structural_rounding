#ifndef solutionIS_hpp
#define solutionIS_hpp

#include <set>
#include <string>
#include <iostream>

class SolutionIS
{
  public:
    SolutionIS();
    std::string filename;
    int n; //nodes
    int k; //degeneracy
    int r; //target degeneracy
    int e; //edges
    double time_vertex_deletion;
    double time_IS_partial_approx;
    double time_lifting;
    double time_IS_full_approx;
    std::set<int> edit_set; //vertices in edit set
    int actual_k; //degeneracy after editing
    std::set<int> IS_partial_approx; //IS approx on editted graph
    std::set<int> IS_lifting_approx; //IS approx after lifting
    std::set<int> IS_full_approx; //IS approx on full graph
    void print();
  private:
    std::string set_to_csv(std::set<int> s);
};

#endif
