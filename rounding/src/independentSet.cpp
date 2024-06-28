#include "../include/independentSet.hpp"

#include <vector>
#include <set>

/**
 * Greedily approximates the max independent set (IS)
 */
std::set<int> IS_approx(Graph *g)
{
  std::set<int> IS;
  std::pair<int,int> v = g->get_min_degree_vertex(true);
  while (v.first != -1)
  {
    IS.insert(v.first);
    std::set<int> neighbors = g->get_neighbors(v.first, true);
    g->disable_vertex(v.first);
    for (auto i : neighbors)
    {
      g->disable_vertex(i);
    }
    v = g->get_min_degree_vertex(true);
  }
  return IS;
}

/**
 * Adds back the delted vertices and trys to add them to the IS
 *
 * Returns the new IS approximation
 */
std::set<int> vertex_addition_IS(Graph *g, std::set<int> x, std::set<int> IS)
{
  for (auto i : x)
  {
    g->enable_vertex(i);
    bool independent = true;
    for (auto j : IS)
    {
      if (g->has_edge(i, j, true)) independent = false;
    }
    if (independent)
    {
      IS.insert(i);
    }
  }
  return IS;
}
