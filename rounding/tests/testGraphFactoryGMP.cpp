#include "catch2/catch.hpp"
#include "../include/graph.hpp"
#include "../include/io.hpp"
#include "../include/degeneracy.hpp"
#include "../include/testUtil.hpp"
#include "../include/graphFactoryMatrixGMP.hpp"

#include <vector>
#include <gmp.h>

TEST_CASE( "pascal triangle (GMP)", "[graphFactory]" ) {
  GraphFactoryMatrixGMP *gf = new GraphFactoryMatrixGMP();
  gf->create_pascal_triangle();
  mpz_t num;
  mpz_init(num);
  char *s = (char*)malloc(sizeof(char) * 20);
  gf->choose(num, 50, 25);
  mpz_get_str(s, 10, num);
  REQUIRE( strcmp(s, "126410606437752") == 0 );
  free(s);
}

TEST_CASE( "n choose k (GMP)", "[graphFactory]" ) {
  GraphFactoryMatrixGMP *gf = new GraphFactoryMatrixGMP();
  mpz_t num;
  mpz_init(num);
  char *s = (char*)malloc(sizeof(char) * 20);
  gf->choose(num, 50, 25);
  mpz_get_str(s, 10, num);
  REQUIRE( strcmp(s, "126410606437752") == 0 );
  free(s);
}

TEST_CASE( "random vector edge case min (GMP)", "[graphFactory]" ) {
  GraphFactoryMatrixGMP *gf = new GraphFactoryMatrixGMP();
  std::vector<int> v;
  mpz_t num;
  mpz_init(num);
  std::vector<int> n = gf->random_vector(14, 4, v, num);
  REQUIRE( n[0]+n[1]+n[2]+n[3] == 14 );
  REQUIRE( n[0] == 0 );
  REQUIRE( n[1] == 0 );
  REQUIRE( n[2] == 0 );
  REQUIRE( n[3] == 14 );
}

TEST_CASE( "random vector edge case max (GMP)", "[graphFactory]" ) {
  GraphFactoryMatrixGMP *gf = new GraphFactoryMatrixGMP();
  gf->create_pascal_triangle();
  std::vector<int> v;
  mpz_t num;
  mpz_init(num);
  gf->choose(num, 14+4-1, 14);
  mpz_sub_ui(num, num, 1);
  std::vector<int> n = gf->random_vector(14, 4, v, num);
  REQUIRE( n[0]+n[1]+n[2]+n[3] == 14 );
  REQUIRE( n[0] == 14 );
  REQUIRE( n[1] == 0 );
  REQUIRE( n[2] == 0 );
  REQUIRE( n[3] == 0 );
}

TEST_CASE( "random vector edge case max+1 (GMP)", "[graphFactory]" ) {
  GraphFactoryMatrixGMP *gf = new GraphFactoryMatrixGMP();
  gf->create_pascal_triangle();
  std::vector<int> v;
  mpz_t num;
  mpz_init(num);
  gf->choose(num, 14+4-1, 14);
  std::vector<int> n = gf->random_vector(14, 4, v, num);
  REQUIRE( n.size() == 0 );
}

TEST_CASE( "random vector edge case all 0s (GMP)", "[graphFactory]" ) {
  GraphFactoryMatrixGMP *gf = new GraphFactoryMatrixGMP();
  gf->create_pascal_triangle();
  std::vector<int> v;
  mpz_t num;
  mpz_init(num);
  std::vector<int> n = gf->random_vector(0, 4, v, num);
  REQUIRE( n.size() == 4 );
  REQUIRE( n[0] == 0);
  REQUIRE( n[1] == 0);
  REQUIRE( n[2] == 0);
  REQUIRE( n[3] == 0);
}

TEST_CASE( "random vector (GMP)", "[graphFactory]" ) {
  GraphFactoryMatrixGMP *gf = new GraphFactoryMatrixGMP();
  std::vector<int> v;
  mpz_t num;
  mpz_init(num);
  mpz_set_ui(num, 394);
  std::vector<int> n = gf->random_vector(14, 4, v, num);
  REQUIRE( n[0]+n[1]+n[2]+n[3] == 14 );
  REQUIRE( n[0] == 4 );
  REQUIRE( n[1] == 0 );
  REQUIRE( n[2] == 0 );
  REQUIRE( n[3] == 10 );

  mpz_set_ui(num, 424);
  n = gf->random_vector(14, 4, v, num);
  REQUIRE( n[0]+n[1]+n[2]+n[3] == 14 );
  REQUIRE( n[0] == 4 );
  REQUIRE( n[1] == 3 );
  REQUIRE( n[2] == 0 );
  REQUIRE( n[3] == 7 );

  mpz_set_ui(num, 426);
  n = gf->random_vector(14, 4, v, num);
  REQUIRE( n[0]+n[1]+n[2]+n[3] == 14 );
  REQUIRE( n[0] == 4 );
  REQUIRE( n[1] == 3 );
  REQUIRE( n[2] == 2 );
  REQUIRE( n[3] == 5 );
}

TEST_CASE( "get min number of edges for a core (GMP)", "[graphFactory]") {
  GraphFactoryMatrixGMP *gf = new GraphFactoryMatrixGMP();
  std::vector<int> n{4, 3, 2, 5};
  std::vector<int> min = gf->get_min_edges(n);
  REQUIRE( min[0] == 2 );
  REQUIRE( min[1] == 3 );
  REQUIRE( min[2] == 5 );
  REQUIRE( min[3] == 10 );
}

TEST_CASE( "get max number of edges for a core (GMP)", "[graphFactory]") {
  GraphFactoryMatrixGMP *gf = new GraphFactoryMatrixGMP();
  std::vector<int> n{4, 3, 2, 5};
  std::vector<int> max = gf->get_max_edges(n);
  REQUIRE( max[0] == 4 );
  REQUIRE( max[1] == 6 );
  REQUIRE( max[2] == 6 );
  REQUIRE( max[3] == 10 );
}

TEST_CASE( "min deg vertex from set: robert.txt (GMP)", "[graphFactory]" ) {
  GraphFactoryMatrixGMP *gf = new GraphFactoryMatrixGMP(false);
  Graph *g = read_edgelist_from_file("tests/graphs/robert.txt");
  std::vector<Vertex *> list;
  for (int i = 0; i < g->get_n(); i++)
  {
    list.push_back(g->get_vertex(i));
  }
  Vertex *v = gf->get_min_deg_from(list);
  REQUIRE( v->deg() == 4 );
}

TEST_CASE( "add inter edge (GMP)", "[graphFactory]" ) {
  GraphFactoryMatrixGMP *gf = new GraphFactoryMatrixGMP(false);
  Graph *g = clique_graph(3);
  std::vector<Vertex *> i_core;
  for (int i = 0; i < g->get_n(); i++)
  {
    i_core.push_back(g->get_vertex(i));
  }
  g = join_graphs(g, clique_graph(4));
  std::vector<Vertex *> j_core;
  for (int i = 3; i < g->get_n(); i++)
  {
    j_core.push_back(g->get_vertex(i));
  }
  int k = 2; //does not matter here
  gf->add_inter_edge(i_core, j_core, k, g);
  REQUIRE( g->get_edges().size() == 10 );
}

TEST_CASE( "add intra edge (GMP)", "[graphFactory]" ) {
  GraphFactoryMatrixGMP *gf = new GraphFactoryMatrixGMP(false);
  Graph *g = clique_graph(3);
  g->grow(2);
  g->add_edge(2, 3);
  g->add_edge(2, 4);
  g->add_edge(3, 4);
  std::vector<Vertex *> core;
  for (int i = 0; i < g->get_n(); i++)
  {
    core.push_back(g->get_vertex(i));
  }
  int k = 3; //does not matter here
  gf->add_intra_edge(core, k, g);
  REQUIRE( g->get_edges().size() == 7 );
}

TEST_CASE( "random core sizes (GMP)", "[graphFactory]" ) {
  GraphFactoryMatrixGMP *gf = new GraphFactoryMatrixGMP();
  std::vector<int> n = gf->random_core_sizes(14, 1, 4);
  REQUIRE( n.size() == 4);
  REQUIRE( n[0]+n[1]+n[2]+n[3] == 14 );
}
