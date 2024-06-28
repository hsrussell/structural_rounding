#ifndef graphFactoryGMP_hpp
#define graphFactoryGMP_hpp

#include "graph.hpp"
#include "vertex.hpp"
#include "io.hpp"
#include <gmp.h>
#include <gmpxx.h>
#include <set>
#include <random>
#include <string>

class GraphFactoryGMP
{
  public:
    GraphFactoryGMP(bool printToFile, unsigned int seed);

    void create_pascal_triangle();

    void choose(mpz_t result, int n, int k);

    std::vector<int> random_vector(
      int n,
      int k,
      std::vector<int> vec,
      mpz_t random
    );

    std::vector<int> random_core_sizes(
      int n,
      int r,
      int k
    );

    virtual Graph *random_graph_by_cores(
      int n,
      int r,
      int k,
      std::string filename = ""
    ) = 0;

    std::vector<int> get_min_edges(std::vector<int> n);

    std::vector<int> get_max_edges(std::vector<int> n);

    bool add_intra_edge(
      std::vector<Vertex *> core,
      int k,
      Graph *g
    );

    bool add_inter_edge(
      std::vector<Vertex *> i_core,
      std::vector<Vertex *> j_core,
      int k,
      Graph *g
    );

    void adjust_edges(
      std::vector<Vertex *> core,
      int i,
      Graph *g
    );

    bool rewire_down_edge(
      std::vector<Vertex *> core,
      int i,
      Graph *g,
      std::vector<std::pair<int,int>> path
    );

    void swap_edge(
      std::vector<Vertex *> core,
      int i,
      Vertex *min_v,
      Graph *g
    );

    std::vector<std::pair<int, int>> edge_random_ordering(
      std::vector<Vertex *> i_core,
      std::vector<Vertex *> j_core
    );

    bool core_contains(std::vector<Vertex *> core, int id);

    Vertex *get_min_deg_from(std::vector<Vertex *> core);

    Vertex *get_max_deg_from(std::vector<Vertex *> core);

  protected:
    bool printToFile;
    int pascal_limit;
    std::vector<std::vector<mpz_class>> pascal;
    gmp_randstate_t state;
    std::default_random_engine reng;
};

#endif
