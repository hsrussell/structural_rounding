#include "../include/graphFactoryGMP.hpp"

#include <algorithm>
#include <limits.h>
#include <random>
#include <stdlib.h>

/**
 * Generates graphs
 */
GraphFactoryGMP::GraphFactoryGMP(bool printToFile, unsigned int seed) :
  printToFile(printToFile), pascal_limit(100)
{
  reng.seed(seed);
  srand(seed);
  gmp_randinit_default ( state );
  gmp_randseed_ui ( state, seed );
}

/**
 * Creates and stores pascal's triangle for fast lookup
 */
void GraphFactoryGMP::create_pascal_triangle()
{
  for(int i = 0; i <= pascal_limit; i++)
  {
    std::vector<mpz_class> v;
    pascal.push_back(v);
    for(int j = 0; j <= i; j++)
    {
      if (j == 0 || j == i)
      {
        mpz_class m = 1;
        pascal[i].push_back(m);
      }
      else
      {
        mpz_class m = pascal[i-1][j-1] + pascal[i-1][j];
        pascal[i].push_back(m);
      }
    }
  }
}

/**
 * n choose k
 * where n >= 0 and k >= 0
 */
void GraphFactoryGMP::choose(mpz_t result, int n, int k)
{
  if (k == 0 || k == n)
  {
    mpz_set_ui(result, 1);
  }
  else if (n == 0 || n < k)
  {
    mpz_set_ui(result, 0);
  }
  //Assuming we want to use pascal's triangle
  else if (!(pascal.size() == 0 || n > pascal_limit || k > pascal_limit))
  {
    mpz_set(result, pascal[n][k].get_mpz_t());
  }
  else
  {
    choose(result, n-1, k-1);
    //cannot divide by 0
    if (mpz_cmp_d(result, 0) == 0) return;
    mpz_mul_ui(result, result, n);
    mpz_tdiv_q_ui(result, result, k);
  }
}

/**
 * n undistiguishable balls in k distinguishable boxes
 * Generates a random vector of length k
 * such that the sum of its elements is n
 * with all possible vectors occuring with equal probability
 *
 * n number of balls
 * k number of cells
 * v existing vector<int>
 * random id combination (allows for additional constraints)
 */
std::vector<int> GraphFactoryGMP::random_vector(
  int n, int k, std::vector<int> vec, mpz_t random
)
{
  // std::cout << n << " " << k << " " << random << std::endl;
  if (k == 0)
  {
    return vec;
  }
  if (k == 1)
  {
    vec.push_back(n);
    return vec;
  }
  if (n == 0)
  {
    vec.push_back(n);
    return random_vector(n, k-1, vec, random);
  }
  int balls = 0;
  mpz_t prev;
  mpz_init(prev);
  while (mpz_cmp_d(random, 0) >= 0)
  {
    if (n-balls < 0)
    {
      std::cerr << "error, random number for vector creation too large" << std::endl;
      return std::vector<int>();
    }
    choose(prev, n-balls+k-2, n-balls);
    mpz_sub(random, random, prev);
    balls++;
  }
  balls--;
  mpz_add(random, random, prev);
  vec.push_back(balls);
  return random_vector(n-balls, k-1, vec, random);
}

/**
 * Generates a random vector denoting the core sizes
 * Requires that the kth core has at least k+1 vertices to be valid
 *
 * n number of vertices
 * r minimum nonempty core
 * k maximum nonempty core
 */
std::vector<int> GraphFactoryGMP::random_core_sizes(
  int n,
  int r,
  int k
)
{
  std::vector<int> core_sizes;
  int last_core_min = k+1;
  int remaining_v = n-last_core_min;
  if (remaining_v < 0)
  {
    std::cerr << "Not enough vertices for degeneracy " << k << std::endl;
    return std::vector<int>();
  }
  int non_empty_cores = k-(r-1);
  mpz_t max_random;
  mpz_init(max_random);
  choose(max_random, remaining_v+non_empty_cores-1, remaining_v);
  mpz_t random;
  mpz_init(random);
  if (mpz_cmp_ui(max_random, 0) != 0) {
    mpz_urandomm (random, state, max_random);
  }
  for (int i = 1; i < r; i++) core_sizes.push_back(0);
  core_sizes = random_vector(remaining_v, non_empty_cores, core_sizes, random);
  core_sizes[k-1] += last_core_min;
  return core_sizes;
}

std::vector<int> GraphFactoryGMP::get_min_edges(std::vector<int> n)
{
  int k = n.size();
  std::vector<int> v;
  for (int i = 0; i < k; i++)
  {
    int core = i + 1;
    int min;
    if (n[i] > core)
    {
      min = (core * n[i] + 1) / 2;
    }
    else
    {
      mpz_t temp;
      mpz_init(temp);
      choose(temp, n[i], 2);
      int num = n[i] * (core - n[i] + 1);
      mpz_add_ui(temp, temp, num);
      //Assumes small values
      unsigned long int tt = mpz_get_ui(temp);
      if (tt > INT_MAX)
      {
        std::cerr << "Too large to convert to int: " << tt << std::endl;
        return std::vector<int>();
      }
      min = (int) tt;
    }
    v.push_back(min);
  }
  return v;
}

std::vector<int> GraphFactoryGMP::get_max_edges(std::vector<int> n)
{
  int k = n.size();
  std::vector<int> v;
  for (int i = 0; i < k; i++)
  {
    int core = i + 1;
    int max;
    if (core < k)
    {
      max = core * n[i];
    }
    else
    {
      max = core * n[i] - (core*core + core) / 2;
    }
    v.push_back(max);
  }
  return v;
}

/**
 * Selects two nonadjacent vertices vi and vj in core i where outdeg(vi) < K
 * and σ(vi) < σ(vj) and adds an edge between them
 */
bool GraphFactoryGMP::add_intra_edge(std::vector<Vertex *> core, int k, Graph *g)
{
  std::vector<std::pair<int, int>> edges = edge_random_ordering(core, core);
  for (auto e : edges)
  {
    if (e.first < e.second &&
      !g->has_edge(e.first, e.second) &&
      g->get_vertex_outward_deg(e.first) < k)
    {
      g->add_edge(e.first, e.second);
      //Since we predefine an ordering on the vertiecs
      //we (vi, vj) is an outward edge, but (vj, vi) is not
      g->incr_outward_deg(e.first);
      return true;
    }
  }
  return false;
}

/**
 * Selects two nonadjacent vertices vi ∈ Vi and vj ∈ Vj where outdeg(vi) < k
 * and adds an edge between them
 */
bool GraphFactoryGMP::add_inter_edge(
  std::vector<Vertex *> i_core, std::vector<Vertex *> j_core, int k, Graph *g
)
{
  std::vector<std::pair<int, int>> edges = edge_random_ordering(i_core, j_core);
  for (auto e : edges)
  {
      if (!g->has_edge(e.first, e.second) &&
          g->get_vertex_outward_deg(e.first) < k)
      {
        g->add_edge(e.first, e.second);
        g->incr_outward_deg(e.first);
        return true;
      }
  }
  return false;
}


/**
 * Swaps and rewires the edges in core i until the min deg
 * of the vertices in core i is at least i
 */
void GraphFactoryGMP::adjust_edges(std::vector<Vertex *> core, int i, Graph *g)
{
  Vertex *min_v = get_min_deg_from(core);
  while (min_v != NULL && min_v->deg() < i)
  {
    std::vector<std::pair<int,int>> path;
    bool done = rewire_down_edge(core, i, g, path);
    if (!done)
    {
      swap_edge(core, i, min_v, g);
    }
    min_v = get_min_deg_from(core);
  }
}

/**
 * Takes an inter-edge {vj,w} (where vj is in core k and w is in
 * some core >k and not in the neighborhood of min_v, the
 * vertex in core k with min deg) and replaces it with {vj,min_v}
 */
bool GraphFactoryGMP::rewire_down_edge(
  std::vector<Vertex *> core,
  int i,
  Graph *g,
  std::vector<std::pair<int,int>> path
)
{
  Vertex *min_v = get_min_deg_from(core);
  if (min_v->deg() >= i) return true;

  std::shuffle(core.begin(), core.end(), reng);
  for (auto vj : core)
  {
    //if ∃{vj,w} ∈ E with vj ∈ Vi \ N(vi) and w ∈ V
    if (vj != min_v && !g->has_edge(vj->id, min_v->id))
    {
      //E ← (E \ {{vj,w}}) ∪ {{vi,vj}}
      std::set<int> neigh = g->get_neighbors(vj->id);
      std::vector<int> neighbors(neigh.begin(), neigh.end());
      std::shuffle(neighbors.begin(), neighbors.end(), reng);
      for (auto n : neighbors)
      {
        bool already_used = false;
        //Assures that n is in core > i
        //since we construct the largest core first
        if (!core_contains(core, n))
        {
          for (auto edge : path)
          {
            if ((edge.first == n && edge.second == vj->id) ||
              (edge.first == vj->id && edge.second == n))
            {
              already_used = true;
              break;
            }
          }
          if (!already_used)
          {
            g->remove_edge(n, vj->id);
            g->add_edge(vj->id, min_v->id);
            g->incr_outward_deg(min_v->id);
            return true;
          }
        }
      }
      for (auto n : neighbors)
      {
        bool already_used = false;
        for (auto edge : path)
        {
          if ((edge.first == n && edge.second == vj->id) ||
            (edge.first == vj->id && edge.second == n))
          {
            already_used = true;
            break;
          }
        }
        if (!already_used)
        {
          //Assures that n is in core i
          //since we construct the largest core first
          g->remove_edge(n, vj->id);
          g->add_edge(vj->id, min_v->id);
          g->incr_outward_deg(min_v->id);
          path.push_back(std::make_pair(n, vj->id));
          bool attempt = rewire_down_edge(core, i, g, path);
          if (attempt) return true;
          //revert graph before trying next edge
          g->add_edge(n, vj->id);
          g->remove_edge(vj->id, min_v->id);
          g->decr_outward_deg(min_v->id);
          path.pop_back();
        }
      }
    }
  }
  return false;
}

/**
 * Given the minimum degree vertex in core i, min_v
 * Selects vj ∈ Vi with deg(vj ) > i
 * Selects a neighbor vj' of vj that is not a neighbor of min_v
 * Replaces edge {vj,vj'} with edge {vi,vj'}
 */
void GraphFactoryGMP::swap_edge(
  std::vector<Vertex *> core,
  int i,
  Vertex *min_v,
  Graph *g
)
{
  //randomly select vj ∈ Vi with deg(vj) > i
  std::shuffle(core.begin(), core.end(), reng);
  for (auto vj : core)
  {
    if (vj->deg() > i)
    {
      //randomly select vj′ ∈ N(vj) \ N(vi)
      std::set<int> neigh = g->get_neighbors(vj->id);
      std::vector<int> neighbors(neigh.begin(), neigh.end());
      std::shuffle(neighbors.begin(), neighbors.end(), reng);
      for (auto vj2 : neighbors)
      {
        if (vj2 != min_v->id && !g->has_edge(vj2, min_v->id))
        {
          // E ← (E \ {{vj , vj′ }}) ∪ {{vi , vj′ }}
          g->remove_edge(vj->id, vj2);
          g->add_edge(min_v->id, vj2);
          return;
        }
      }
    }
  }
}

std::vector<std::pair<int, int>> GraphFactoryGMP::edge_random_ordering(
  std::vector<Vertex *> i_core,
  std::vector<Vertex *> j_core
)
{
  std::vector<std::pair<int, int>> edge_list;
  for (auto i : i_core)
  {
    for (auto j : j_core)
    {
      edge_list.push_back(std::make_pair(i->id, j->id));
    }
  }
  std::shuffle(edge_list.begin(), edge_list.end(), reng);
  return edge_list;
}

bool GraphFactoryGMP::core_contains(std::vector<Vertex *> core, int id)
{
  for (auto i : core)
  {
    if (i->id == id) return true;
  }
  return false;
}

Vertex *GraphFactoryGMP::get_min_deg_from(std::vector<Vertex *> core)
{
  if (core.size() == 0) return NULL;
  Vertex *v = core[0];
  int min = v->deg();
  for (auto i : core)
  {
    if (min > i->deg())
    {
      min = i->deg();
      v = i;
    }
  }
  return v;
}

Vertex *GraphFactoryGMP::get_max_deg_from(std::vector<Vertex *> core)
{
  if (core.size() == 0) return NULL;
  Vertex *v = core[0];
  int max = v->deg();
  for (auto i : core)
  {
    if (max < i->deg())
    {
      max = i->deg();
      v = i;
    }
  }
  return v;
}
