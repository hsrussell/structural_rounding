#ifndef dominatingSet_hpp
#define dominatingSet_hpp

#include "graph.hpp"
#include <set>

std::set<int> DS_greedy_approx(Graph *g);
void DS_rule_1(Graph *g, std::vector<int> vertices);
void DS_rule_2(Graph *g, int d, std::vector<int> d_order, int k);
void DS_rule_2_p(Graph *g, std::vector<int> d_order, int p, int d, int k);
std::set<int> find_common_neighbors(
  Graph *g,
  std::set<int> vertices,
  int min_size
);
void DS_rule_2_p_subsets(
  Graph *g,
  std::set<int> subset,
  std::vector<int> neighbors,
  int i,
  int size,
  int ssize
);
void DS_rule_2_steps(Graph *g, std::set<int> set_B, std::set<int> set_U, int size);
void DS_rule_3(Graph *g, std::vector<int> vertices, int d, int k);
std::set<int> count_color(Graph *g, int color, std::set<int> vertices);
int horners_rule(int a, int x, int b);
void DS_rule_4(Graph *g, std::vector<int> vertices);
void DS_rule_5(Graph *g);
bool DS_rule_6(Graph *g, int d, int k);
std::set<int> min_DS(Graph *g);
std::vector<int> find_min_DS(
  Graph *g,
  std::vector<int> d_set,
  std::vector<int> min_d_set,
  std::vector<int> vertices,
  int i
);
Graph *DS_poly_kernel(Graph *g, int k);
std::set<int> DS_fpt_approx(Graph *g, int k);

std::set<int> vertex_addition_DS(Graph *g, std::set<int> x, std::set<int> DS);

#endif
