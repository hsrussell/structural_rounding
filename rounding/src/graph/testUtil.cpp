#include "../../include/testUtil.hpp"

/**
 * Returns the union of two graphs
 * on disjoint sets of vertices
 */
Graph* join_graphs(Graph *a, Graph *b)
{
  int a_n = a->get_n();
  a->grow(b->get_n());
  std::set<std::pair<int,int>> b_edges = b->get_edges();
  for (auto i : b_edges)
  {
    a->add_edge(i.first + a_n, i.second + a_n);
  }
  return a;
}

/**
 * Returns a clique on k vertices
 */
Graph *clique_graph(int k)
{
  Graph *g = new Graph(k);
  for (int i = 0; i < k; i++)
  {
    for (int j = i+1; j < k; j++)
    {
      g->add_edge(i, j);
    }
  }
  return g;
}
