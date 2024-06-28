#include "catch2/catch.hpp"
#include "../include/graph.hpp"
#include "../include/io.hpp"

#include <vector>

TEST_CASE( "read edgelist from file: wikiGraph.txt", "[graph]" ) {
  Graph *g = read_edgelist_from_file("tests/graphs/wikiGraph.txt");
  CHECK( g != NULL );
  REQUIRE( g->has_edge(15, 14, true) );
  REQUIRE( g->has_edge(14, 10, true) );
  REQUIRE( g->has_edge(13, 4, true) );
  REQUIRE( g->has_edge(12, 3, true) );
  REQUIRE( g->has_edge(11, 10, true) );
  REQUIRE( g->has_edge(10, 5, true) );
  REQUIRE( g->has_edge(9, 3, true) );
  REQUIRE( g->has_edge(9, 4, true) );
  REQUIRE( g->has_edge(8, 5, true) );
  REQUIRE( g->has_edge(8, 2, true) );
  REQUIRE( g->has_edge(7, 5, true) );
  REQUIRE( g->has_edge(7, 6, true) );
  REQUIRE( g->has_edge(6, 5, true) );
  REQUIRE( g->has_edge(6, 4, true) );
  REQUIRE( g->has_edge(5, 4, true) );
  REQUIRE( g->has_edge(5, 3, true) );
  REQUIRE( g->has_edge(4, 3, true) );
  REQUIRE( g->has_edge(2, 1, true) );
  REQUIRE( g->has_edge(2, 0, true) );
  REQUIRE( g->has_edge(1, 0, true) );
  REQUIRE( g->get_n() == 16 );
}

TEST_CASE( "read adjacencylist from file: wikiGraphAdj.txt", "[graph]" ) {
  Graph *g = read_adjacencylist_from_file("tests/graphs/wikiGraphAdj.txt");
  CHECK( g != NULL );
  REQUIRE( g->has_edge(15, 14, true) );
  REQUIRE( g->has_edge(14, 10, true) );
  REQUIRE( g->has_edge(13, 4, true) );
  REQUIRE( g->has_edge(12, 3, true) );
  REQUIRE( g->has_edge(11, 10, true) );
  REQUIRE( g->has_edge(10, 5, true) );
  REQUIRE( g->has_edge(9, 3, true) );
  REQUIRE( g->has_edge(9, 4, true) );
  REQUIRE( g->has_edge(8, 5, true) );
  REQUIRE( g->has_edge(8, 2, true) );
  REQUIRE( g->has_edge(7, 5, true) );
  REQUIRE( g->has_edge(7, 6, true) );
  REQUIRE( g->has_edge(6, 5, true) );
  REQUIRE( g->has_edge(6, 4, true) );
  REQUIRE( g->has_edge(5, 4, true) );
  REQUIRE( g->has_edge(5, 3, true) );
  REQUIRE( g->has_edge(4, 3, true) );
  REQUIRE( g->has_edge(2, 1, true) );
  REQUIRE( g->has_edge(2, 0, true) );
  REQUIRE( g->has_edge(1, 0, true) );
  REQUIRE( g->get_n() == 16 );
}

TEST_CASE( "graph symmetry", "[graph]" ) {
  Graph *g = read_edgelist_from_file("tests/graphs/wikiGraph.txt");
  REQUIRE( g->has_edge(15, 14, true) );
  REQUIRE( g->has_edge(14, 15, true) );
  REQUIRE( g->has_edge(10, 5, true) );
  REQUIRE( g->has_edge(5, 10, true) );
}

TEST_CASE( "remove vertex", "[graph]" ) {
  Graph *g = read_edgelist_from_file("tests/graphs/wikiGraph.txt");
  g->disable_vertex(5);
  REQUIRE( g->get_neighbors(0, true).size()  == 2 );
  REQUIRE( g->get_neighbors(1, true).size()  == 2 );
  REQUIRE( g->get_neighbors(2, true).size()  == 3 );
  REQUIRE( g->get_neighbors(3, true).size()  == 3 );
  REQUIRE( g->get_neighbors(4, true).size()  == 4 );
  REQUIRE( g->get_neighbors(5, true).size()  == 0 );
  REQUIRE( g->get_neighbors(6, true).size()  == 2 );
  REQUIRE( g->get_neighbors(7, true).size()  == 1 );
  REQUIRE( g->get_neighbors(8, true).size()  == 1 );
  REQUIRE( g->get_neighbors(9, true).size()  == 2 );
  REQUIRE( g->get_neighbors(10, true).size() == 2 );
  REQUIRE( g->get_neighbors(11, true).size() == 1 );
  REQUIRE( g->get_neighbors(12, true).size() == 1 );
  REQUIRE( g->get_neighbors(13, true).size() == 1 );
  REQUIRE( g->get_neighbors(14, true).size() == 2 );
  REQUIRE( g->get_neighbors(15, true).size() == 1 );
  REQUIRE( g->has_edge(15, 14, true) );
  REQUIRE( g->has_edge(14, 10, true) );
  REQUIRE( g->has_edge(13, 4, true) );
  REQUIRE( g->has_edge(12, 3, true) );
  REQUIRE( g->has_edge(11, 10, true) );
  REQUIRE( g->has_edge(9, 3, true) );
  REQUIRE( g->has_edge(9, 4, true) );
  REQUIRE( g->has_edge(8, 2, true) );
  REQUIRE( g->has_edge(7, 6, true) );
  REQUIRE( g->has_edge(6, 4, true) );
  REQUIRE( g->has_edge(4, 3, true) );
  REQUIRE( g->has_edge(2, 1, true) );
  REQUIRE( g->has_edge(2, 0, true) );
  REQUIRE( g->has_edge(1, 0, true) );
  REQUIRE( !g->has_edge(10, 5, true) );
  REQUIRE( !g->has_edge(8, 5, true) );
  REQUIRE( !g->has_edge(7, 5, true) );
  REQUIRE( !g->has_edge(6, 5, true) );
  REQUIRE( !g->has_edge(5, 4, true) );
  REQUIRE( !g->has_edge(5, 3, true) );
}

TEST_CASE( "vertex degrees", "[graph]" ) {
  Graph *g = read_edgelist_from_file("tests/graphs/wikiGraph.txt");
  REQUIRE( g->get_vertex(0)->deg()  == 2 );
  REQUIRE( g->get_vertex(1)->deg()  == 2 );
  REQUIRE( g->get_vertex(2)->deg()  == 3 );
  REQUIRE( g->get_vertex(3)->deg()  == 4 );
  REQUIRE( g->get_vertex(4)->deg()  == 5 );
  REQUIRE( g->get_vertex(5)->deg()  == 6 );
  REQUIRE( g->get_vertex(6)->deg()  == 3 );
  REQUIRE( g->get_vertex(7)->deg()  == 2 );
  REQUIRE( g->get_vertex(8)->deg()  == 2 );
  REQUIRE( g->get_vertex(9)->deg()  == 2 );
  REQUIRE( g->get_vertex(10)->deg() == 3 );
  REQUIRE( g->get_vertex(11)->deg() == 1 );
  REQUIRE( g->get_vertex(12)->deg() == 1 );
  REQUIRE( g->get_vertex(13)->deg() == 1 );
  REQUIRE( g->get_vertex(14)->deg() == 2 );
  REQUIRE( g->get_vertex(15)->deg() == 1 );
}

TEST_CASE( "vertex min degree order", "[graph]" ) {
  Graph *g = read_edgelist_from_file("tests/graphs/wikiGraph.txt");
  std::vector<int> order = g->get_vertex_min_deg_order();
  REQUIRE( order[0]  == 11 );
  REQUIRE( order[1]  == 12 );
  REQUIRE( order[2]  == 13 );
  REQUIRE( order[3]  == 15 );
  REQUIRE( order[4]  == 0 );
  REQUIRE( order[5]  == 1 );
  REQUIRE( order[6]  == 7 );
  REQUIRE( order[7]  == 8 );
  REQUIRE( order[8]  == 9 );
  REQUIRE( order[9]  == 14 );
  REQUIRE( order[10] == 2 );
  REQUIRE( order[11] == 6 );
  REQUIRE( order[12] == 10 );
  REQUIRE( order[13] == 3 );
  REQUIRE( order[14] == 4 );
  REQUIRE( order[15] == 5 );
}

TEST_CASE( "remove vertex, then vertex min degree order", "[graph]" ) {
  Graph *g = read_edgelist_from_file("tests/graphs/wikiGraph.txt");
  g->disable_vertex(5);
  std::vector<int> order = g->get_vertex_min_deg_order();
  REQUIRE( order[0]  == 7 );
  REQUIRE( order[1]  == 8 );
  REQUIRE( order[2]  == 11 );
  REQUIRE( order[3]  == 12 );
  REQUIRE( order[4]  == 13 );
  REQUIRE( order[5]  == 15 );
  REQUIRE( order[6]  == 0 );
  REQUIRE( order[7]  == 1 );
  REQUIRE( order[8]  == 6 );
  REQUIRE( order[9]  == 9 );
  REQUIRE( order[10] == 10 );
  REQUIRE( order[11] == 14 );
  REQUIRE( order[12] == 2 );
  REQUIRE( order[13] == 3 );
  REQUIRE( order[14] == 4 );
}
