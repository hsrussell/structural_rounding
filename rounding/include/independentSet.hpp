#ifndef independentSet_hpp
#define independentSet_hpp

#include "graph.hpp"

std::set<int> IS_approx(Graph *g);

std::set<int> vertex_addition_IS(Graph *g, std::set<int> x, std::set<int> IS);

#endif
