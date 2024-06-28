#include "../include/graphFactoryMatrixGMP.hpp"
#include <limits.h>
#include <stdlib.h>

GraphFactoryMatrixGMP::GraphFactoryMatrixGMP(bool printToFile, unsigned int seed) :
  GraphFactoryGMP(printToFile, seed) {}

Graph *GraphFactoryMatrixGMP::random_graph_by_cores(
  int n, int r, int k, std::string filename
)
{
  Graph *g = new Graph();
  if (pascal.size() == 0) create_pascal_triangle();
  std::vector<int> core_sizes = random_core_sizes(n, r, k);
  if (core_sizes.size() == 0)
  {
    return NULL;
  }
  std::vector<int> min = get_min_edges(core_sizes);
  std::vector<int> max = get_max_edges(core_sizes);
  std::vector<std::vector<int>> matrix = random_matrix(core_sizes, min, max);
  if (matrix.size() == 0) return NULL;
  g = core_generator(core_sizes, matrix);
  if (printToFile) write_adjacencylist_to_file(filename, g, false);
  return g;
}

/**
 * Creates an nxn upper triangular matrix M
 * where Mij is the number of edges from core i to core j
 */
std::vector<std::vector<int>> GraphFactoryMatrixGMP::random_matrix(
  std::vector<int> n,
  std::vector<int> min,
  std::vector<int> max
)
{
  int k = n.size();
  std::vector<std::vector<int>> matrix;
  for (int i = 0; i < k; i++)
  {
    int num_edges;
    //These are the bounds of the number of outward edges
    if (max[i]+1 == min[i])
    {
      //cannot % by 0
      num_edges = min[i];
    }
    else
    {
      num_edges = rand() % (max[i]+1-min[i]) + min[i];
    }
    std::vector<int> v = random_matrix_vector(num_edges, i+1, n, k);
    //couldn't find a valid vector within in the limit
    if (v.size() == 0) return std::vector<std::vector<int>>();

    for (int j = 0; j < i; j++) {
      v.insert(v.begin(), 0);
    }
    //Creating the ith row of the matrix
    matrix.push_back(v);
  }
  return matrix;
}

/**
 * Returns a matrix row for core i with randomized edges
 * considering the necessary (but not guarenteed sufficient)
 * bounds on intra and inter edges
 *
 * e number of edges in core i
 * i core number
 * n number of nodes in core i
 * v existing vector of zeros for valid vector length in matrix
 */
std::vector<int> GraphFactoryMatrixGMP::random_matrix_vector(
  int e,
  int i,
  std::vector<int> core_sizes,
  int k
)
{
  std::vector<int> v;
  int n = core_sizes[i-1];
  //find the empty cores
  int core_holes = 0;
  for (int j = i; j < k; j++)
  {
    if (core_sizes[j] == 0)
    {
      core_holes++;
    }
  }
  //set bounds for inter and intra edges
  int intra_max = 0;
  int intra_min = n * i - e;
  if (n == 1)
  {
    intra_max = 0;
    intra_min = 0;
  }
  else if (i == k)
  {
    intra_min = e;
    intra_max = e;
  }
  else if (n > i)
  {
    intra_max = (n - i) * i + i * (i - 1) / 2;
  }
  else
  {
    mpz_t temp1;
    mpz_init(temp1);
    choose(temp1, n, 2);
    unsigned long int tt = mpz_get_ui(temp1);
    if (tt > INT_MAX)
    {
      std::cerr << "Too large to convert to int: " << tt << std::endl;
      return std::vector<int>();
    }
    intra_max = (int) tt;
  }
  if (intra_max > e)
  {
    intra_max = e;
  }
  intra_max++;
  //decide on number of intra edges
  int intra_edges = 0;
  if (intra_max == intra_min)
  {
    //cannot % by 0
    intra_edges = intra_min;
  }
  else
  {
    intra_edges = rand() % (intra_max - intra_min) + intra_min;
  }
  v.push_back(intra_edges);
  //define inter edges
  int inter_edges = e - intra_edges;
  mpz_t random;
  mpz_init(random);
  mpz_t max_random;
  mpz_init(max_random);
  choose(max_random, inter_edges + (k-i-core_holes) - 1, inter_edges);
  bool all_valid = false;
  std::vector<int> temp = v;
  int loop_max = 1000000;
  int counter = 0;
  std::vector<int> no_holes;
  while (!all_valid)
  {
    counter++;
    if (counter % loop_max == 0)
    {
      std::cerr << "waiting..." << std::endl;
    }
    if (mpz_cmp_ui(max_random, 0) == 0)
    {
      //cannot % by 0
      mpz_set_ui(random, 0);
    }
    else
    {
      mpz_urandomm (random, state, max_random);
    }
    v = temp;
    no_holes = random_vector(inter_edges, (k-i-core_holes), std::vector<int>(), random);
    int index = 0;
    for (int j = i; j < k; j++)
    {
      if (core_sizes[j] == 0)
      {
        v.push_back(0);
      }
      else
      {
        v.push_back(no_holes[index]);
        index++;
      }
    }
    //ensure that we aren't trying to have more edges
    //to core j than reasonable for the number of vertices
    //in core j
    all_valid = true;
    if (n != 0)
    {
      for (int j = 1; j < k-i; j++)
      {
        if (v[j] > (core_sizes[i+j-1] / n))
        {
          all_valid = false;
          break;
        }
      }
    }
  }
  if (loop_max == 0)
  {
    return std::vector<int>();
  }
  return v;
}

/**
 * Generates a random graph with exaclty degeneracy k
 * by building each core
 *
 * n[i] number of vertices in core i
 * matrix k x k upper triangular (or symmetric) matrix
 *    where matrix[a][b] is the number of edges
 *    between core i and core j
 */
Graph *GraphFactoryMatrixGMP::core_generator(
  std::vector<int> n,
  std::vector<std::vector<int>> matrix
)
{
  int k = n.size();
  Graph *g = new Graph();
  std::vector<std::vector<Vertex *>> cores;
  //create the empty cores
  for (int i = 0; i < k; i++)
  {
    std::vector<Vertex *> c;
    cores.push_back(c);
    // create Ni new nodes and store them in Vi define an ordering σ on Vi
    for (int j = 0; j < n[i]; j++)
    {
      Vertex *v = new Vertex(g->get_n());
      cores[i].push_back(v);
      g->add_vertex(v);
    }
  }
  for (int i = k-1; i >= 0; i--)
  {
    for (int j = i; j < k; j++)
    {
      int m_entry = matrix[i][j];
      for (int m = 1; m <= m_entry; m++)
      {
        if (g->get_min_outward_deg() >= k)
        {
          //error!
          std::cerr << "Error, too many edges are requested!\n" << std::endl;
          return NULL;
        }
        if (i == j)
        {
          add_intra_edge(cores[i], i+1, g);
        }
        else
        {
          add_inter_edge(cores[i], cores[j], i+1, g);
        }
      }
    }
    adjust_edges(cores[i], i+1, g);
  }
  return g;
}
