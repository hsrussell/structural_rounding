#include "../include/vertexDeletion.hpp"

#include <algorithm>
#include <cfloat>
#include <vector>

/**
 * Approximation for r-degenerate vertex deletion set
 *
 * Returns the set of vertices to delete to obtain
 *    a graph with degeneracy r
 */
std::set<int> local_ratio_recursion(Graph *g, int r, float b)
{
  std::vector<int> order = g->get_vertex_min_deg_order();
  //if V(G) = ∅, return ∅
  if (order.size() == 0) return std::set<int>();
  float rb = r * b;
  //if ∃v∈V(G) where degG(v) ≤ rb
  for (auto i : order)
  {
    if (g->get_neighbors(i, true).size() <= rb)
    {
      g->disable_vertex(i);
      //localRatioRecursion(G\{v}, w, r, b)
      std::set<int> x = local_ratio_recursion(g, r, b);
      //revert to original G
      g->enable_vertex(i);
      return x;
    }
  }
  //if ∃v∈V(G) where w(v) = 0
  for (auto i : order)
  {
    if (g->get_vertex(i)->weight == 0)
    {
      g->disable_vertex(i);
      //x ← localRatioRecursion(G\{v}, w, r, b)
      std::set<int> x = local_ratio_recursion(g, r, b);
      //revert to original G
      g->enable_vertex(i);
      //G\x
      for (auto j : x) g->disable_vertex(j);
      //if G\x has degeneracy rb
      if (find_degeneracy(g) <= rb)
      {
        //revert to original G
        for (auto j : x) g->enable_vertex(j);
        return x;
      }
      else
      {
        //revert to original G
        for (auto j : x) g->enable_vertex(j);
        // X U {v}
        x.insert(i);

        //minimalSolution(G, XU{v}, w, r, b);
        return minimal_solution(g, x, rb);
      }
    }
  }

  //ε := min w(v) / degG(v) over all v∈V(G)
  float epsilon = FLT_MAX;
  for (auto i : order)
  {
    Vertex *v = g->get_vertex(i);
    epsilon = std::min(epsilon, v->weight / v->deg());
  }
  //Define w1(u) := ε · degG(u) for all u ∈ V
  //Define w2 := w − w1.
  for (auto i : order)
  {
    Vertex *v = g->get_vertex(i);
    v->weight -= epsilon * v->deg();
    // v->set_weight(v->weight - epsilon * v->deg());
    return local_ratio_recursion(g, r, b);
  }
  //should not make it here
  std::cout << "Something went very wrong.";
  return std::set<int>();
}

std::set<int> minimal_solution(Graph *g, std::set<int> x, float rb)
{
  //hardcopy, not great for very large x.size(), but prevents loop errors
  std::set<int> x_original = x;
  for (auto i : x_original)
  {
    //X\{v}
    x.erase(i);
    //G\(X\{v})
    for (auto j : x) g->disable_vertex(j);
    //if G\(X\{v}) has degeneracy rb
    if (find_degeneracy(g) <= rb)
    {
      //revert to original G
      for (auto j : x) g->enable_vertex(j);
      return minimal_solution(g, x, rb);
    }
    //revert to original G
    for (auto j : x) g->enable_vertex(j);
    //revert to original x
    x.insert(i);
  }
  return x;
}
