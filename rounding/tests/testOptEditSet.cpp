#include "catch2/catch.hpp"
#include "../include/graph.hpp"
#include "../include/io.hpp"
#include "../include/testUtil.hpp"
#include "../include/optEditSet.hpp"

#include <set>

TEST_CASE( "optEditSet: wikiGraph", "[optEditSet]" ) {
  Graph *g = read_edgelist_from_file("tests/graphs/wikiGraph.txt");
  std::set<int> opt = opt_edit_set(g,2);
  REQUIRE( opt.size() == 0);
}

TEST_CASE( "optEditSet: bobLazyAdj", "[optEditSet]" ) {
  Graph *g = read_adjacencylist_from_file("tests/graphs/bobLazyAdj.txt");
  std::set<int> opt = opt_edit_set(g,2);
  REQUIRE( opt.size() == 3);
}

TEST_CASE( "optEditSet: bobLazy2Adj", "[optEditSet]" ) {
  Graph *g = read_adjacencylist_from_file("tests/graphs/bobLazy2Adj.txt");
  std::set<int> opt = opt_edit_set(g,2);
  REQUIRE( opt.size() == 5);
}

TEST_CASE( "optEditSet: K5", "[optEditSet]" ) {
  Graph *g = clique_graph(5);
  std::set<int> opt = opt_edit_set(g,2);
  REQUIRE( opt.size() == 2);
}

TEST_CASE( "optEditSet: K6", "[optEditSet]" ) {
  Graph *g = clique_graph(6);
  std::set<int> opt = opt_edit_set(g,2);
  REQUIRE( opt.size() == 3);
}

TEST_CASE( "optEditSet: K7", "[optEditSet]" ) {
  Graph *g = clique_graph(7);
  std::set<int> opt = opt_edit_set(g,2);
  REQUIRE( opt.size() == 4);
}
