#include "../include/dominatingSet.hpp"
#include "../include/graph.hpp"
#include "../include/degeneracy.hpp"

#include <vector>
#include <algorithm>
#include <set>

/**
 * Greedily approximates the min dominating set (DS)
 */
std::set<int> DS_greedy_approx(Graph *g)
{
  std::set<int> DS;
  std::pair<int,int> v = g->get_max_degree_vertex(true);
  while (v.first != -1)
  {
    DS.insert(v.first);
    std::set<int> neighbors = g->get_neighbors(v.first,true);
    g->disable_vertex(v.first);
    for (auto n : neighbors)
    {
      g->disable_vertex(n);
    }
    v = g->get_max_degree_vertex(true);
  }
  return DS;
}

// Rule 1
// Color all isolated black vertices of G red.
void DS_rule_1(Graph *g, std::vector<int> vertices)
{
  for (auto v : vertices)
  {
    if (g->get_neighbors(v, true).size() == 0)
    {
      g->get_vertex(v)->color = 2;
    }
  }
}

// Rule 2
// For p = 1,2,...,i−2, in this order,
// apply Rule 2.p repeatedly till it no
// longer causes any changes in the graph.
void DS_rule_2(Graph *g, int d, std::vector<int> d_order, int k)
{
  for (int p = 1; p <= d-1; p++)
  {
    DS_rule_2_p(g, d_order, p, d, k);
  }
}

// Rule 2.p
// Let b = jk if p = 1, and
// b = jk^{p} + k^{p−1} + k^{p−2} + ··· + k
// if 2 ≤ p ≤ i−2. If a set of (i−p) vertices
// U = {u_1,u_2,...,u_{i−p}}, none of which is red,
// has more than b common black neighbors,
// then let B be this set of neighbors.
// 1. Color all the vertices in B white.
// 2. Add to the graph (i − p) new (gadget)
//    vertices X = {x_1,x_2,...,x_{i−p}} and
//    all the edges {u,x}, u ∈ U, x ∈ X
// 3. Color all the vertices in X black.
void DS_rule_2_p(Graph *g, std::vector<int> d_order, int p, int d, int k)
{
  // for l := 1 to n
  for (auto v : d_order)
  {
    // if vl is black and its degree in G[vl+1,...,vn] is at least d−p+1
    // Find the neighborhood N of vl in G[vl+1, ... , vn]
    std::set<int> neighbors = g->get_neighbors(v, true);
    if (g->get_vertex(v)->color == 0 && neighbors.size() > d-p)
    {
      int ssize = horners_rule(d+1, k, p);
      std::vector<int> neigh_vec(neighbors.begin(), neighbors.end());
      DS_rule_2_p_subsets(g, std::set<int>(), neigh_vec, 0, d-p+1, ssize);
    }
  }
}

std::set<int> find_common_neighbors(Graph *g, std::set<int> vertices, int min_size)
{
  std::vector<int> result = g->get_vertices();
  std::vector<int> buffer;
  for (auto v : vertices)
  {
    buffer.clear();
    std::set<int> temp = g->get_neighbors(v, true);
    std::set_intersection(result.begin(), result.end(),
      temp.begin(), temp.end(), std::back_inserter(buffer));
    swap(result, buffer);
    if (result.size() <= min_size)
    {
      return std::set<int>();
    }
  }
  std::set<int> intersection(result.begin(), result.end());
  return intersection;
}

// for each (d−p+1)-subset S of N
void DS_rule_2_p_subsets(
  Graph *g,
  std::set<int> subset,
  std::vector<int> neighbors,
  int i,
  int size, //d - p + 1
  int ssize
)
{
  if (subset.size() == size)
  {
    // if S has more than (d+1)k^{p} + k^{p−1} + ··· + k
    // common black neighbors in G
    std::set<int> set_B = find_common_neighbors(g, subset, ssize);
    if (set_B.size() > ssize)
    {
      // Apply the three steps of Rule 2.p, taking S as U
      DS_rule_2_steps(g, set_B, subset, size-1);
    }

  }
  else if (i < neighbors.size())
  {
    //without
    DS_rule_2_p_subsets(g, subset, neighbors, i+1, size, ssize);
    subset.insert(neighbors[i]);
    //with
    DS_rule_2_p_subsets(g, subset, neighbors, i+1, size, ssize);
    subset.erase(subset.find(neighbors[i]));
  }
}

// 1. Color all the vertices in B white.
// 2. Add to the graph (i − p) new (gadget)
//    vertices X = {x_1,x_2,...,x_{i−p}} and
//    all the edges {u,x}, u ∈ U, x ∈ X
// 3. Color all the vertices in X black.
void DS_rule_2_steps(Graph *g, std::set<int> set_B, std::set<int> set_U, int size)
{
  for (auto v : set_B)
  {
    g->get_vertex(v)->color = 1;
  }
  for (int i = 0; i < size; i++)
  {
    int id = g->get_n();
    for (auto v : set_U)
    {
      g->add_edge(v, id);
    }
  }
  // new vertices default to black
}

// Rule 3
// If a black or white vertex u has more than
// jk^{i−1} + k^{i−2} + ··· + k^2 + k black neighbors,
// then color u red and color all the black
// neighbors of u white.
void DS_rule_3(Graph *g, std::vector<int> vertices, int d, int k)
{
  int size = horners_rule(d+1, k, d);
  for (auto u : vertices)
  {
    if (g->get_vertex(u)->color != 2)
    {
      std::set<int> neighbors = g->get_neighbors(u, true);
      std::set<int> black_neighbors = count_color(g, 0, neighbors);
      if (black_neighbors.size() > size)
      {
        g->get_vertex(u)->color = 2;
        for (auto v : black_neighbors)
        {
          g->get_vertex(v)->color = 1;
        }
      }
    }
  }
}

std::set<int> count_color(Graph *g, int color, std::set<int> vertices)
{
  std::set<int> subset;
  for (auto v : vertices)
  {
    if (g->get_vertex(v)->color == color)
    {
      subset.insert(v);
    }
  }
  return subset;
}

//Rule 2: (d+1)k^{p} + k^{p−1} + ··· + k
//Rule 3: jk^{i−1} + k^{i−2} + ··· + k^2 + k
//General case: ax^{b} + x^{b-1} + ··· + x^{2} + x
int horners_rule(int a, int x, int b)
{
  int ret = a;
  for (int i = 0; i < b; i++)
  {
    ret *= x;
    ret += 1;
  }
  if (b > 0)
  {
    ret--;
  }
  return ret;
}

// Rule 4
// If a white vertex u is adjacent to at most
// one black vertex, then delete u and apply Rule 1.
void DS_rule_4(Graph *g, std::vector<int> vertices)
{
  for (auto v : vertices)
  {
    if (g->get_vertex(v)->color == 1)
    {
      std::set<int> neighbors = g->get_neighbors(v, true);
      std::set<int> black_neighbors = count_color(g, 0, neighbors);
      if (black_neighbors.size() < 2)
      {
        g->disable_vertex(v);
        DS_rule_1(g, g->get_vertices(true));
      }
    }
  }
}

// Rule 5
// If there is a white vertex u and a white or
// black vertex v such that N(u) ∩ BG ⊆ N(v) ∩ BG,
// then delete u and apply Rule 1.
void DS_rule_5(Graph *g)
{
  std::vector<int> vertices = g->get_vertices(true);
  std::set<int> white;
  std::set<int> white_or_black;
  for (auto v : vertices)
  {
    if (g->get_vertex(v)->color == 1)
    {
      white.insert(v);
      white_or_black.insert(v);
    }
    else if (g->get_vertex(v)->color == 0)
    {
      white_or_black.insert(v);
    }
  }
  for (auto w : white)
  {
    for (auto bw : white_or_black)
    {
      if (w != bw)
      {
        std::set<int> neigh_w = g->get_neighbors(w, true);
        std::set<int> b_neigh_w = count_color(g, 0, neigh_w);
        std::set<int> neigh_bw = g->get_neighbors(bw, true);
        std::set<int> b_neigh_bw = count_color(g, 0, neigh_bw);
        bool is_subset = true;
        for (auto n : b_neigh_w)
        {
          if (b_neigh_bw.find(n) == b_neigh_bw.end())
          {
            is_subset = false;
            break;
          }
        }
        if (is_subset)
        {
          g->disable_vertex(w);
          DS_rule_1(g, g->get_vertices());
        }
      }
    }
  }
}

// Rule 6
// If |RG| > k or
// |BG| > jk^{i} + k^{i−1} + k^{i−2} + ··· + k^{2} then output“NO”.
// returns false if impossible
//         otherwise possibility is undetermined
bool DS_rule_6(Graph *g, int d, int k)
{
  std::vector<int> vertices = g->get_vertices(true);
  std::set<int> red;
  int black = 0;
  for (auto v : vertices)
  {
    if (g->get_vertex(v)->color == 2)
    {
      red.insert(v);
    }
    else if (g->get_vertex(v)->color = 0)
    {
      black++;
    }
  }
  int size = horners_rule(d+1, k, d+1);
  if (red.size() > k) return false;
  if (black > size) return false;
  return true;
}

std::set<int> min_DS(Graph *g)
{
  std::vector<int> ds;
  std::vector<int> all_vertices = g->get_vertices(true);
  for (auto v : all_vertices)
  {
    if (g->get_vertex(v)->color == 2)
    {
      ds.push_back(v);
      g->disable_vertex(v);
    }
    if (g->get_vertex(v)->color == 1)
    {
      ds.push_back(v);
      std::set<int> neighbors = g->get_neighbors(v, true);
      g->disable_vertex(v);
      for (auto n : neighbors)
      {
        g->disable_vertex(n);
      }
    }
  }
  std::vector<int> vertices = g->get_vertices(true);
  std::vector<int> opt = find_min_DS(g, ds, all_vertices, vertices, 0);
  return std::set<int>(opt.begin(), opt.end());
}

std::vector<int> find_min_DS(
  Graph *g,
  std::vector<int> d_set,
  std::vector<int> min_d_set,
  std::vector<int> vertices,
  int i
)
{
  if (d_set.size() >= min_d_set.size())
  {
    return min_d_set;
  }
  if (g->get_vertices().size() == 0)
  {
    return d_set;
  }
  if (i == vertices.size())
  {
    return min_d_set;
  }
  int v = vertices[i];
  // with this vertex
  d_set.push_back(v);
  std::set<int> neighbors = g->get_neighbors(v, true, true);
  bool active = g->active_vertex(v);
  g->disable_vertex(v);
  for (auto n : neighbors)
  {
    g->disable_vertex(n);
  }
  std::vector<int> set_with = find_min_DS(g, d_set, min_d_set, vertices, i+1);
  d_set.pop_back();
  if (active) g->enable_vertex(v);
  for (auto n : neighbors)
  {
    g->enable_vertex(n);
  }
  if (set_with.size() < min_d_set.size())
  {
    min_d_set = set_with;
  }
  // without this vertex
  std::vector<int> set_without = find_min_DS(g, d_set, min_d_set, vertices, i+1);
  if (set_without.size() < min_d_set.size())
  {
    min_d_set = set_without;
  }
  return min_d_set;
}

/**
 * Finds the kernel of the graph
 */
Graph *DS_poly_kernel(Graph *g, int k)
{
  std::vector<int> vertices = g->get_vertices(true);
  std::pair<std::vector<std::set<int>>,std::vector<int>> degen =
    find_degeneracy_order(g);
  int d = degen.first.size();
  std::vector<int> d_order = degen.second;

  DS_rule_1(g, vertices);
  DS_rule_2(g, d, d_order, k);
  DS_rule_3(g, vertices, d, k);
  DS_rule_4(g, vertices);
  DS_rule_5(g);
  bool not_impossible = DS_rule_6(g, d, k);
  if (!not_impossible)
  {
    std::cerr << "No such DS of size " << k << " exists." << std::endl;
    return new Graph();
  }
  return g;
}

/**
 * Assumes all the vertices in the graph are initially all black
 */
std::set<int> DS_fpt_approx(Graph *g, int k)
{
  g = DS_poly_kernel(g, k);
  if (g->get_n() > 0)
  {
    return min_DS(g);
  }
  return std::set<int>();
}

/**
 * Adds back the delted vertices and trys to add them to the DS
 *
 * Returns the new DS approximation
 */
std::set<int> vertex_addition_DS(Graph *g, std::set<int> x, std::set<int> DS)
{
  for (auto i : x)
  {
    g->enable_vertex(i);
    bool dominated = false;
    for (auto j : DS)
    {
      if (g->has_edge(i, j))
      {
        dominated = true;
        break;
      }
    }
    if (!dominated)
    {
      DS.insert(i);
    }
  }
  return DS;
}
