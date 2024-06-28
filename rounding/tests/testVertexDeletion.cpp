#include "catch2/catch.hpp"
#include "../include/graph.hpp"
#include "../include/vertexDeletion.hpp"
#include "../include/testUtil.hpp"

#include <vector>
#include <set>

TEST_CASE( "vertex deletion: {}", "[vertexDeletion]" ) {
  Graph *g = new Graph(0);
  std::set<int> s = local_ratio_recursion(g, 2, 1);
  REQUIRE( s.size() == 0 );
}

TEST_CASE( "minimal solution: K5", "[vertexDeletion]" ) {
  Graph *g = clique_graph(5);
  std::set<int> x = std::set<int>();
  x.insert(0);
  x.insert(1);
  std::set<int> s = minimal_solution(g, x, 2);
  REQUIRE( s.size() == 2 );
}

TEST_CASE( "minimal solution with extra vertices: K5", "[vertexDeletion]" ) {
  Graph *g = clique_graph(5);
  std::set<int> x = std::set<int>();
  x.insert(0);
  x.insert(1);
  x.insert(2);
  x.insert(3);
  std::set<int> s = minimal_solution(g, x, 2);
  REQUIRE( s.size() == 2 );
}

TEST_CASE( "vertex deletion: K5", "[vertexDeletion]" ) {
  Graph *g = clique_graph(5);
  std::set<int> s = local_ratio_recursion(g, 2, 1);
  REQUIRE( s.size() == 2 );
}
