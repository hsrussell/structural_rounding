#ifndef optEditSet_hpp
#define optEditSet_hpp

#include "../include/graph.hpp"
#include <set>
#include <vector>

std::set<int> opt_edit_set(
  Graph *g,
  int r
);

std::vector<int> find_edit_set_with(
  Graph *g,
  int r,
  std::vector<int> edit,
  std::vector<int> min_edit,
  std::vector<int> vertices,
  int i
);

std::vector<int> find_edit_set_without(
  Graph *g,
  int r,
  std::vector<int> edit,
  std::vector<int> min_edit,
  std::vector<int> vertices,
  int i
);

std::vector<int> remove_small_cores(Graph *g, int r);

#endif
