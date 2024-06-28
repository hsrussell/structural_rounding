#include "catch2/catch.hpp"
#include "../include/graph.hpp"
#include "../include/io.hpp"
#include "../include/degeneracy.hpp"
#include "../include/vertexDeletion.hpp"
#include "../include/independentSet.hpp"
#include "../include/graphFactoryMatrixGMP.hpp"

#include <vector>

TEST_CASE( "independent set approx: wikiGraph.txt", "[independentSet]" ) {
  Graph *g = read_edgelist_from_file("tests/graphs/wikiGraph.txt");
  std::set<int> s = IS_approx(g);
  REQUIRE( s.find(0)  != s.end() );
  REQUIRE( s.find(6)  != s.end() );
  REQUIRE( s.find(8)  != s.end() );
  REQUIRE( s.find(9)  != s.end() );
  REQUIRE( s.find(11) != s.end() );
  REQUIRE( s.find(12) != s.end() );
  REQUIRE( s.find(13) != s.end() );
  REQUIRE( s.find(14) != s.end() );
  REQUIRE( s.size() == 8 );
}

TEST_CASE( "full pipeline", "[independentSet]" ) {
  Graph *g = read_edgelist_from_file("tests/graphs/robert.txt");
  //step 1: get vertex set to delete
  std::set<int> s = local_ratio_recursion(g, 2, 1);
  //step 2: delete set from graph g
  for (auto i : s) g->disable_vertex(i);
  //step 3: IS set approximation
  std::set<int> IS = IS_approx(g);
  //step 4: re-enable the vertices in the IS
  for (auto i : IS) g->enable_vertex(i);
  //step 5: lifting alg
  std::set<int> ISMore = vertex_addition_IS(g, s, IS);
  REQUIRE( s.size() == 9 );
  REQUIRE( IS.size() == 12 );
  REQUIRE( ISMore.size() == 13 );
}

TEST_CASE( "full pipeline with self generated graph", "[independentSet]" ) {
  GraphFactoryMatrixGMP *gf = new GraphFactoryMatrixGMP();
  Graph *g = gf->random_graph_by_cores(14, 1, 4);
  //step 1: get vertex set to delete
  std::set<int> s = local_ratio_recursion(g, 2, 1);
  //step 2: delete set from graph g
  for (auto i : s) g->disable_vertex(i);
  //step 3: IS set approximation
  std::set<int> IS = IS_approx(g);
  //step 4: re-enable the vertices in the IS
  for (auto i : IS) g->enable_vertex(i);
  //step 5: lifting alg
  std::set<int> ISMore = vertex_addition_IS(g, s, IS);
  REQUIRE( IS.size() <= ISMore.size() );
}
