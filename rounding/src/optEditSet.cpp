#include "../include/optEditSet.hpp"
#include "../include/degeneracy.hpp"

#include <vector>
#include <algorithm>

///////////////////////////////////////////////////////////////////////////////
// Speed
// 17 nodes, 3 opt edit size (0.041s)
// 26 nodes, 5 opt edit size (3.534s)
// 34 nodes, 7 opt edit size (7m0.537s)
// 40 nodes, 9 opt edit size (estimated 6 hours)
///////////////////////////////////////////////////////////////////////////////

/**
 * Finds the minimum number of nodes to delete to reduce a graph
 * to degeneracy r
 *
 * g graph to edit
 * r target degeneracy
 *
 * returns the optimal edit set
 */
std::set<int> opt_edit_set(Graph *g, int r)
{
  if (find_degeneracy(g) == r)
  {
    return std::set<int>();
  }
  std::vector<int> vertices = g->get_vertex_min_deg_order();
  std::reverse(vertices.begin(), vertices.end());
  std::vector<int> opt = find_edit_set_without(g, r, std::vector<int>(), vertices, vertices, 0);
  return std::set<int>(opt.begin(), opt.end());
}

/**
 * Recursively finds the minimum edit set to reduce a graph to degeneracy r
 * after a vertex has been deleted
 *
 * g graph to edit
 * r target degeneracy
 * edit vertices already removed
 * min_edit minimum sized edit set solution found so far
 * vertices vector of the graph vertices
 * i index of the vertex to look at next
 *
 * returns the minimum edit set given that the vertices in edit
 *    have already been removed from the graph
 */
std::vector<int> find_edit_set_with(
  Graph *g,
  int r,
  std::vector<int> edit,
  std::vector<int> min_edit,
  std::vector<int> vertices,
  int i
)
{
  if (edit.size() >= min_edit.size())
  {
    return min_edit;
  }
  if (g->get_vertices().size() == 0)
  {
    min_edit = edit;
  }
  if (i == vertices.size())
  {
    return min_edit;
  }
  //with deleting this vertex
  if (g->active_vertex(vertices[i]))
  {
    g->disable_vertex(vertices[i]);
    edit.push_back(vertices[i]);
    std::vector<int> removed = remove_small_cores(g,r);
    std::vector<int> set_with = find_edit_set_with(g,r,edit,min_edit,vertices,i+1);
    g->enable_vertex(vertices[i]);
    for (auto v : removed)
    {
      g->enable_vertex(v);
    }
    edit.pop_back();
    if (set_with.size() < min_edit.size())
    {
      min_edit = set_with;
    }
  }
  //without deleting this vertex
  std::vector<int> set_without = find_edit_set_without(g,r,edit,min_edit,vertices,i+1);
  if (set_without.size() < min_edit.size())
  {
    min_edit = set_without;
  }
  return min_edit;
}

/**
 * Recursively finds the minimum edit set to reduce a graph to degeneracy r
 * after no vertex has been deleted
 *
 * g graph to edit
 * r target degeneracy
 * edit vertices already removed
 * min_edit minimum sized edit set solution found so far
 * vertices vector of the graph vertices
 * i index of the vertex to look at next
 *
 * returns the minimum edit set given that the vertices in edit
 *    have already been removed from the graph
 */
std::vector<int> find_edit_set_without(
  Graph *g,
  int r,
  std::vector<int> edit,
  std::vector<int> min_edit,
  std::vector<int> vertices,
  int i
)
{
  if (i == vertices.size())
  {
    return min_edit;
  }

  //with deleting this vertex
  if (g->active_vertex(vertices[i]))
  {
    g->disable_vertex(vertices[i]);
    edit.push_back(vertices[i]);
    std::vector<int> removed = remove_small_cores(g,r);
    std::vector<int> set_with = find_edit_set_with(g,r,edit,min_edit,vertices,i+1);
    g->enable_vertex(vertices[i]);
    for (auto v : removed)
    {
      g->enable_vertex(v);
    }
    edit.pop_back();
    if (set_with.size() < min_edit.size())
    {
      min_edit = set_with;
    }
  }
  //without deleting this vertex
  std::vector<int> set_without = find_edit_set_without(g,r,edit,min_edit,vertices,i+1);
  if (set_without.size() < min_edit.size())
  {
    min_edit = set_without;
  }
  return min_edit;
}

/**
 * Removes all vertices in cores smaller than or equal to r
 *
 * g graph to edit
 * r largest core number that will be removed from the graph
 *
 * returns the removed vertices
 */
std::vector<int> remove_small_cores(Graph *g, int r)
{
  std::vector<int> removed;
  std::pair<int,int> min_v = g->get_min_degree_vertex(true);
  while (min_v.first != -1 && min_v.second <= r)
  {
    std::vector<int> active = g->get_vertices(true);
    for (auto a : active)
    {
      if (g->get_neighbors(a, true).size() <= r)
      {
        removed.push_back(a);
        g->disable_vertex(a);
      }
    }
    min_v = g->get_min_degree_vertex(true);
  }
  return removed;
}
