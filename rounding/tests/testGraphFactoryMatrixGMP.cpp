#include "catch2/catch.hpp"
#include "../include/graph.hpp"
#include "../include/io.hpp"
#include "../include/degeneracy.hpp"
#include "../include/testUtil.hpp"
#include "../include/graphFactoryGMP.hpp"
#include "../include/graphFactoryMatrixGMP.hpp"

#include <vector>
#include <gmp.h>

TEST_CASE("random matrix vector (GMP)", "[graphFactorMatrix]" ) {
  GraphFactoryMatrixGMP *gf = new GraphFactoryMatrixGMP();
  std::vector<int> n{4, 3, 2, 5};
  int e = 4;
  int i = 1;
  int k = 4;
  std::vector<int> ret = gf->random_matrix_vector(e, i, n, k);
  REQUIRE( ret.size() == k-i+1 );
  REQUIRE( ret[0] >= n[0] * i - e );
  REQUIRE( ret[0] <= (n[0] - i) * i + i * (i - 1) / 2 );
  int sum = 0;
  for (auto j : ret) sum += j;
  REQUIRE( sum == e );

  e = 4;
  i++;
  ret = gf->random_matrix_vector(e, i, n, k);
  REQUIRE( ret.size() == k-i+1 );
  mpz_t num;
  mpz_init(num);
  gf->choose(num, n[1], 2);
  REQUIRE( mpz_cmp_ui(num, ret[0]) >= 0 );
  REQUIRE( ret[0] >= n[1] * i - e );
  sum = 0;
  for (auto j : ret) sum += j;
  REQUIRE( sum == e );

  e = 6;
  i++;
  ret = gf->random_matrix_vector(e, i, n, k);
  REQUIRE( ret.size() == k-i+1 );
  REQUIRE( ret[0] >= n[2] * i - e );
  gf->choose(num, n[2], 2);
  REQUIRE( mpz_cmp_ui(num, ret[0]) >= 0 );
  sum = 0;
  for (auto j : ret) sum += j;
  REQUIRE( sum == e );

  e = 10;
  i++;
  ret = gf->random_matrix_vector(e, i, n, k);
  REQUIRE( ret.size() == k-i+1 );
  REQUIRE( ret[0] >= n[3] * i - e );
  REQUIRE( ret[0] <= (n[3] - i) * i + i * (i - 1) / 2 );
  sum = 0;
  for (auto j : ret) sum += j;
  REQUIRE( sum == e );
}

TEST_CASE( "random matrix (GMP)", "[graphFactoryMatrix]" ) {
  GraphFactoryMatrixGMP *gf = new GraphFactoryMatrixGMP();
  std::vector<int> n = gf->random_core_sizes(14, 1, 4);
  std::vector<int> min = gf->get_min_edges(n);
  std::vector<int> max = gf->get_max_edges(n);
  std::vector<std::vector<int>> m = gf->random_matrix(n, min, max);
  REQUIRE( m[0].size() == 4 );
  REQUIRE( m[1].size() == 4 );
  REQUIRE( m[2].size() == 4 );
  REQUIRE( m[3].size() == 4 );
}

TEST_CASE( "random matrix set n vector (GMP)", "[graphFactoryMatrix]" ) {
  GraphFactoryMatrixGMP *gf = new GraphFactoryMatrixGMP();
  std::vector<int> n{4, 3, 2, 5};
  std::vector<int> min = gf->get_min_edges(n);
  std::vector<int> max = gf->get_max_edges(n);
  std::vector<std::vector<int>> m = gf->random_matrix(n, min, max);
  REQUIRE( m[0].size() == 4 );
  REQUIRE( m[1].size() == 4 );
  REQUIRE( m[2].size() == 4 );
  REQUIRE( m[3].size() == 4 );
  for (int i = 0; i < 4; i++) {
    int sum = 0;
    for (int j = i; j < 4; j++) {
      sum += m[i][j];
    }
    REQUIRE( sum >= min[i] );
    REQUIRE( sum <= max[i] );
  }
}

TEST_CASE( "core generator matrix (GMP)", "[graphFactoryMatrix]" ) {
  GraphFactoryMatrixGMP *gf = new GraphFactoryMatrixGMP(false);
  std::vector<int> n{4, 3, 2, 5};
  std::vector<std::vector<int>> matrix;
  std::vector<int> m1{3, 1, 0, 0};
  std::vector<int> m2{1, 2, 2, 0};
  std::vector<int> m3{0, 2, 0, 6};
  std::vector<int> m4{0, 0, 6, 10};
  matrix.push_back(m1);
  matrix.push_back(m2);
  matrix.push_back(m3);
  matrix.push_back(m4);
  Graph *g = gf->core_generator(n, matrix);
  REQUIRE( find_degeneracy(g) == 4 );
}

TEST_CASE( "random graph by cores matrix (GMP)", "[graphFactoryMatrix]" ) {
  GraphFactoryMatrixGMP *gf = new GraphFactoryMatrixGMP();
  Graph *g = gf->random_graph_by_cores(14, 1, 4);
  REQUIRE( find_degeneracy(g) == 4 );
}
