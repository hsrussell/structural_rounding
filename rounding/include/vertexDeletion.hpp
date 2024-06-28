#ifndef vertexDeletion_hpp
#define vertexDeletion_hpp

#include "graph.hpp"
#include "degeneracy.hpp"
#include "vertex.hpp"

std::set<int> local_ratio_recursion(Graph *g, int r, float b);

std::set<int> minimal_solution(Graph *g, std::set<int> x, float rb);

#endif
