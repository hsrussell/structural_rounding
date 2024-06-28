#include "catch2/catch.hpp"
#include "../include/graph.hpp"
#include "../include/io.hpp"
#include "../include/testUtil.hpp"
#include "../include/degeneracy.hpp"

#include <vector>

TEST_CASE( "degeneracy order: wikiGraph", "[degeneracy]" ) {
  Graph *g = read_edgelist_from_file("tests/graphs/wikiGraph.txt");
  std::pair<std::vector<std::set<int>>,std::vector<int>> degen =
    find_degeneracy_order(g);
  std::vector<int> order = degen.second;
  REQUIRE( degen.first.size() == 2 );
  REQUIRE( order[0]  == 11 );
  REQUIRE( order[1]  == 12 );
  REQUIRE( order[2]  == 13 );
  REQUIRE( order[3]  == 15 );
  REQUIRE( order[4]  == 14 );
  REQUIRE( order[5]  == 10 );
  REQUIRE( order[6]  == 0  );
  REQUIRE( order[7]  == 1  );
  REQUIRE( order[8]  == 2  );
  REQUIRE( order[9]  == 8  );
  REQUIRE( order[10] == 7  );
  REQUIRE( order[11] == 9  );
  REQUIRE( order[12] == 6  );
  REQUIRE( order[13] == 3  );
  REQUIRE( order[14] == 5  );
  REQUIRE( order[15] == 4  );
}

TEST_CASE( "degeneracy: wikiGraph", "[degeneracy]" ) {
  Graph *g = read_edgelist_from_file("tests/graphs/wikiGraph.txt");
  REQUIRE( find_degeneracy(g) == 2);
}

TEST_CASE( "degeneracy: K5", "[degeneracy]" ) {
  Graph *g = clique_graph(5);
  REQUIRE( find_degeneracy(g) == 4);
}

TEST_CASE( "degeneracy with disabled vertices: K5", "[degeneracy]" ) {
  Graph *g = clique_graph(5);
  g->disable_vertex(0);
  g->disable_vertex(1);
  REQUIRE( find_degeneracy(g) == 2);
}
