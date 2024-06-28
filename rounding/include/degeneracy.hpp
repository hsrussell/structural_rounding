#ifndef degeneracy_hpp
#define degeneracy_hpp

#include "graph.hpp"

#include <vector>
#include <utility>

int find_degeneracy(Graph *g);

std::pair<std::vector<std::set<int>>,std::vector<int>> find_degeneracy_order(Graph *g);

#endif
