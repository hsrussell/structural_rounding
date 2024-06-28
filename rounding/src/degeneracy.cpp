#include "../include/degeneracy.hpp"

/**
 * Determines k such that graph G is k-degenerate by
 * repeatedly removing the vertex with the lowest degree from G
 * where the degree of the vertices are updated after each removal.
 * k is the maximum degree of a vertex upon removal.
 */
int find_degeneracy(Graph *g)
{
  if (g == NULL) return 0;
  std::pair<std::vector<std::set<int>>,std::vector<int>> ret = find_degeneracy_order(g);
  return ret.first.size();
}

/**
 * Returns the cores
 *         a vertex ordering
 */
std::pair<std::vector<std::set<int>>,std::vector<int>> find_degeneracy_order(Graph *g)
{
  int n = g->get_n();
  std::vector<std::vector<int>> buckets(n, std::vector<int>());
  std::vector<int> vertices = g->get_vertices();
  std::vector<std::set<int>> cores;
  std::vector<int> degeneracy_ordering;

  int bucket_max = 0;
  for (auto i : vertices)
  {
    int deg = g->get_neighbors(i, true).size();
    buckets[deg].push_back(i);
    bucket_max = std::max(bucket_max, deg);
  }

  int dg = -1;
  int id = -1;
  for (int j = 0; j < n; j++)
  {
    while (buckets[j].size() != 0)
    {
      if (dg < j)
      {
        dg = j;
        while (cores.size() <= dg)
        {
          cores.push_back(std::set<int>());
        }
      }
      id = *buckets[j].begin();
      buckets[j].erase(buckets[j].begin());
      cores[dg].insert(id);
      degeneracy_ordering.push_back(id);
      for (int k = j; k < n; k++)
      {
        for (int i = 0; i < buckets[k].size(); i++)
        {
          if (g->has_edge(id, buckets[k][i], true))
          {
            buckets[k-1].push_back(buckets[k][i]);
            buckets[k].erase(buckets[k].begin()+i);
            i--;
          }
        }
      }
      j--;
      j = std::max(0, j);
    }
  }
  cores.erase(cores.begin());
  return std::make_pair(cores, degeneracy_ordering);
}
