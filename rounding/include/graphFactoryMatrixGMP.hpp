#ifndef graphFactoryMatrixGMP_hpp
#define graphFactoryMatrixGMP_hpp

#include "graph.hpp"
#include "graphFactoryGMP.hpp"
#include "vertex.hpp"
#include "io.hpp"
#include <set>
#include <gmp.h>
#include <utility>
#include <string>

class GraphFactoryMatrixGMP: public GraphFactoryGMP
{
  public:
    GraphFactoryMatrixGMP(bool printToFile = false, unsigned int seed = time(0));

    Graph *random_graph_by_cores(
      int n,
      int r,
      int k,
      std::string filename = ""
    );

    std::vector<int> random_matrix_vector(
      int e,
      int i,
      std::vector<int> n,
      int k
    );

    std::vector<std::vector<int>> random_matrix(
      std::vector<int> n,
      std::vector<int> min,
      std::vector<int> max
    );

    Graph *core_generator(
      std::vector<int> n,
      std::vector<std::vector<int>> matrix
    );
};

#endif
