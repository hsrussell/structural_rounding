#include "catch2/catch.hpp"
#include "../include/graph.hpp"
#include "../include/io.hpp"
#include "../include/testUtil.hpp"
#include "../include/degeneracy.hpp"
#include "../include/dominatingSet.hpp"

#include <set>

TEST_CASE( "DS greedy approx: wikiGraph", "[dominatingSet]" ) {
  Graph *g = read_edgelist_from_file("tests/graphs/wikiGraph.txt");
  std::set<int> approx = DS_greedy_approx(g);
  REQUIRE( approx.size() == 7 );
  REQUIRE( approx.find(5)  != approx.end() );
  REQUIRE( approx.find(0)  != approx.end() );
  REQUIRE( approx.find(14) != approx.end() );
  REQUIRE( approx.find(9)  != approx.end() );
  REQUIRE( approx.find(11) != approx.end() );
  REQUIRE( approx.find(12) != approx.end() );
  REQUIRE( approx.find(13) != approx.end() );
}

TEST_CASE( "DS rule 1: wikiGraph no change", "[dominatingSet]" ) {
  Graph *g = read_edgelist_from_file("tests/graphs/wikiGraph.txt");
  std::vector<int> vertices = g->get_vertices();
  DS_rule_1(g, vertices);
  bool all_black = true;
  for (auto v : vertices)
  {
    if (g->get_vertex(v)->color != 0)
    {
      all_black = false;
      break;
    }
  }
  REQUIRE( all_black == true );
}

TEST_CASE( "DS rule 1: single vertex", "[dominatingSet]" ) {
  Graph *g = new Graph(1);
  std::vector<int> vertices = g->get_vertices();
  DS_rule_1(g, vertices);
  REQUIRE( g->get_vertex(0)->color == 2 );
}

TEST_CASE( "DS find common neighbors: wikiGraph", "[dominatingSet]" ) {
  Graph *g = read_edgelist_from_file("tests/graphs/wikiGraph.txt");
  std::set<int> set1;
  set1.insert(5);
  set1.insert(6);
  std::set<int> intersection = find_common_neighbors(g, set1, 0);
  REQUIRE( intersection.size() == 2 );
  REQUIRE( intersection.find(7) != intersection.end() );
  REQUIRE( intersection.find(4) != intersection.end() );
}

TEST_CASE( "DS rule 2 steps: wikiGraph", "[dominatingSet]" ) {
  Graph *g = read_edgelist_from_file("tests/graphs/wikiGraph.txt");
  //Not actually a legitimate situation, but it tests the method
  std::set<int> set_U;
  set_U.insert(5);
  set_U.insert(6);
  std::set<int> set_B;
  set_B.insert(7);
  set_B.insert(4);
  int size = 2; // i-p = d+1-p
  DS_rule_2_steps(g, set_B, set_U, size);
  REQUIRE( g->get_n() == 18 );
  REQUIRE( g->has_edge(5, 16) );
  REQUIRE( g->has_edge(5, 17) );
  REQUIRE( g->has_edge(6, 16) );
  REQUIRE( g->has_edge(6, 17) );
}

TEST_CASE( "DS rule 2 p subset: wikiGraph", "[dominatingSet]" ) {
  Graph *g = read_edgelist_from_file("tests/graphs/wikiGraph.txt");
  //Not actually a legitimate situation, but it tests the method
  //the ordering is not legitimate
  //the second 2 is not legitimate
  std::set<int> neighbors = g->get_neighbors(5, true);
  std::vector<int> neigh_vec(neighbors.begin(), neighbors.end());
  DS_rule_2_p_subsets(g, std::set<int>(), neigh_vec, 0, 2, 1);
  REQUIRE( g->get_n() == 19 );
  REQUIRE( g->has_edge(4, 16) );
  REQUIRE( g->has_edge(7, 16) );
  REQUIRE( g->has_edge(3, 17) );
  REQUIRE( g->has_edge(6, 17) );
  REQUIRE( g->has_edge(3, 18) );
  REQUIRE( g->has_edge(4, 18) );
}

TEST_CASE( "DS rule 2 p: wikiGraph no change", "[dominatingSet]" ) {
  Graph *g = read_edgelist_from_file("tests/graphs/wikiGraph.txt");
  std::pair<std::vector<std::set<int>>,std::vector<int>> degen = find_degeneracy_order(g);
  std::vector<int> order = degen.second;
  int p = 1;
  int d = 2;
  int k = 7;
  DS_rule_2_p(g, order, p, d, k);
  REQUIRE( g->get_n() == 16 );
}

TEST_CASE( "DS rule 2 p=1: benLazyAdj", "[dominatingSet]" ) {
  Graph *g = read_adjacencylist_from_file("tests/graphs/benLazyAdj.txt");
  std::pair<std::vector<std::set<int>>,std::vector<int>> degen = find_degeneracy_order(g);
  std::vector<int> order = degen.second;
  //Not actually a legitimate situation, but it tests the method
  int p = 1;
  int d = 2;
  int k = 2;
  DS_rule_2_p(g, order, p, d, k);
  REQUIRE( g->get_n() == 11 );
  REQUIRE( g->has_edge(1, 9 ) );
  REQUIRE( g->has_edge(1, 10) );
  REQUIRE( g->has_edge(2, 9 ) );
  REQUIRE( g->has_edge(2, 10) );
}

TEST_CASE( "DS rule 2: benLazyAdj", "[dominatingSet]" ) {
  Graph *g = read_adjacencylist_from_file("tests/graphs/benLazyAdj.txt");
  std::pair<std::vector<std::set<int>>,std::vector<int>> degen = find_degeneracy_order(g);
  std::vector<int> order = degen.second;
  //Not actually a legitimate situation, but it tests the method
  int p = 1;
  int d = 2;
  int k = 2;
  DS_rule_2(g, d, order, k);
  REQUIRE( g->get_n() == 11 );
  REQUIRE( g->has_edge(1, 9 ) );
  REQUIRE( g->has_edge(1, 10) );
  REQUIRE( g->has_edge(2, 9 ) );
  REQUIRE( g->has_edge(2, 10) );
}

TEST_CASE( "DS rule 3: benLazyAdj", "[dominatingSet]" ) {
  Graph *g = read_adjacencylist_from_file("tests/graphs/benLazyAdj.txt");
  std::vector<int> vertices = g->get_vertices();
  //Not actually a legitimate situation, but it tests the method
  int d = 2;
  int k = 1;
  DS_rule_3(g, vertices, d, k);
  REQUIRE( g->get_vertex(1)->color == 2 );
  REQUIRE( g->get_vertex(2)->color == 1 );
  REQUIRE( g->get_vertex(3)->color == 1 );
  REQUIRE( g->get_vertex(4)->color == 1 );
  REQUIRE( g->get_vertex(5)->color == 1 );
  REQUIRE( g->get_vertex(6)->color == 1 );
  REQUIRE( g->get_vertex(7)->color == 1 );
  REQUIRE( g->get_vertex(8)->color == 1 );
  REQUIRE( g->get_vertex(9)->color == 1 );
}

TEST_CASE( "DS rule 4: S4", "[dominatingSet]" ) {
  Graph *g = new Graph(5);
  g->add_edge(0, 1);
  g->add_edge(0, 2);
  g->add_edge(0, 3);
  g->add_edge(0, 4);
  g->get_vertex(1)->color = 1;
  g->get_vertex(2)->color = 1;
  g->get_vertex(3)->color = 1;
  g->get_vertex(4)->color = 1;
  std::vector<int> vertices = g->get_vertices();
  DS_rule_4(g, vertices);
  REQUIRE( g->get_neighbors(0, true).size() == 0 );
  REQUIRE( g->get_vertex(0)->color == 2 );
  REQUIRE( g->get_vertex(1)->active == false );
  REQUIRE( g->get_vertex(2)->active == false );
  REQUIRE( g->get_vertex(3)->active == false );
  REQUIRE( g->get_vertex(4)->active == false );
}

TEST_CASE( "DS rule 5: P4", "[dominatingSet]" ) {
  Graph *g = new Graph(4);
  g->add_edge(0, 1);
  g->add_edge(1, 2);
  g->add_edge(2, 3);
  g->add_edge(3, 4);
  g->get_vertex(0)->color = 1;
  DS_rule_5(g);
  REQUIRE( g->get_vertex(0)->active == false );
}

// TEST_CASE( "DS rule 6: Red(G) > k", "[dominatingSet]" ) {
// }

TEST_CASE( "Horners rule", "[dominatingSet]" ) {
  int a = 2;
  int b = 3;
  int x = 4;
  int ret0 = horners_rule(a, x, b);
  int ret1 = horners_rule(0, x, b);
  int ret2 = horners_rule(a, 0, b);
  int ret3 = horners_rule(a, x, 0);
  REQUIRE( ret0 == 148 );
  REQUIRE( ret1 == 20  );
  REQUIRE( ret2 == 0   );
  REQUIRE( ret3 == 2   );
}

TEST_CASE( "min DS: colored benAfter5Adj.txt", "[dominatingSet]" ) {
  Graph *g = read_adjacencylist_from_file("tests/graphs/benAfter5Adj.txt");
  g->get_vertex(7)->color  = 1;
  g->get_vertex(8)->color  = 2;
  g->get_vertex(12)->color = 1;
  g->get_vertex(13)->color = 2;
  std::set<int> ds = min_DS(g);
  REQUIRE( ds.size() == 6 );
  REQUIRE( ds.find(5)  != ds.end() );
  REQUIRE( ds.find(6)  != ds.end() );
  REQUIRE( ds.find(7)  != ds.end() );
  REQUIRE( ds.find(8)  != ds.end() );
  REQUIRE( ds.find(12) != ds.end() );
  REQUIRE( ds.find(13) != ds.end() );
}

TEST_CASE( "min DS: all black benAfter5Adj.txt", "[dominatingSet]" ) {
  Graph *g = read_adjacencylist_from_file("tests/graphs/benAfter5Adj.txt");
  std::set<int> ds = min_DS(g);
  REQUIRE( ds.size() == 5 );
  REQUIRE( ds.find(11)  != ds.end() );
  REQUIRE( ds.find(6)  != ds.end() );
  REQUIRE( ds.find(7)  != ds.end() );
  REQUIRE( ds.find(12) != ds.end() );
  REQUIRE( ds.find(13) != ds.end() );
}

TEST_CASE( "DS rule 5: benAfter4Adj.txt", "[dominatingSet]" ) {
  Graph *g = read_adjacencylist_from_file("tests/graphs/benAfter4Adj.txt");
  g->get_vertex(7)->color  = 1;
  g->get_vertex(8)->color  = 2;
  g->get_vertex(12)->color = 1;
  g->get_vertex(13)->color = 2;
  g->get_vertex(17)->color = 1;
  g->get_vertex(18)->color = 1;
  g->get_vertex(19)->color = 1;
  for (int i = 20; i < 45; i++)
  {
    g->get_vertex(i)->color = 1;
  }
  DS_rule_5(g);
  std::set<int> ds = min_DS(g);
  REQUIRE( ds.size() == 6 );
  REQUIRE( ds.find(5)  != ds.end() );
  REQUIRE( ds.find(6)  != ds.end() );
  REQUIRE( ds.find(7)  != ds.end() );
  REQUIRE( ds.find(8)  != ds.end() );
  REQUIRE( ds.find(12) != ds.end() );
  REQUIRE( ds.find(13) != ds.end() );
}

TEST_CASE( "DS rule 4: benAfter3Adj.txt", "[dominatingSet]" ) {
  Graph *g = read_adjacencylist_from_file("tests/graphs/benAfter3Adj.txt");
  g->get_vertex(7)->color  = 1;
  g->get_vertex(8)->color  = 2;
  g->get_vertex(12)->color = 1;
  g->get_vertex(13)->color = 2;
  g->get_vertex(17)->color = 1;
  g->get_vertex(18)->color = 1;
  g->get_vertex(19)->color = 1;
  for (int i = 20; i < 1130; i++)
  {
    g->get_vertex(i)->color = 1;
  }
  std::vector<int> vertices = g->get_vertices();
  DS_rule_4(g, vertices);
  vertices = g->get_vertices();
  REQUIRE( vertices.size() == 45 );
  write_adjacencylist_to_file("/temp.txt", g, true);
  DS_rule_5(g);
  std::set<int> ds = min_DS(g);
  REQUIRE( ds.size() == 6 );
  REQUIRE( ds.find(5)  != ds.end() );
  REQUIRE( ds.find(6)  != ds.end() );
  REQUIRE( ds.find(7)  != ds.end() );
  REQUIRE( ds.find(8)  != ds.end() );
  REQUIRE( ds.find(12) != ds.end() );
  REQUIRE( ds.find(13) != ds.end() );
}

TEST_CASE( "DS rule 3: benAfter2Adj.txt", "[dominatingSet]" ) {
  Graph *g = read_adjacencylist_from_file("tests/graphs/benAfter2Adj.txt");
  g->get_vertex(8)->color = 1;
  g->get_vertex(18)->color = 1;
  for (int i = 20; i < 226; i++)
  {
    g->get_vertex(i)->color = 1;
  }
  std::vector<int> vertices = g->get_vertices();
  DS_rule_3(g, vertices, 3, 6);
  for (int i = 226; i < 1130; i++)
  {
    REQUIRE( g->get_vertex(i)->color == 1 );
  }
  REQUIRE( g->get_vertex(7)->color  == 1 );
  REQUIRE( g->get_vertex(17)->color == 1 );
  REQUIRE( g->get_vertex(8)->color  == 2 );
  REQUIRE( g->get_vertex(12)->color == 1 );
  REQUIRE( g->get_vertex(19)->color == 1 );
  DS_rule_4(g, vertices);
  vertices = g->get_vertices();
  REQUIRE( vertices.size() == 45 );
  write_adjacencylist_to_file("/temp.txt", g, true);
  DS_rule_5(g);
  std::set<int> ds = min_DS(g);
  REQUIRE( ds.size() == 6 );
  REQUIRE( ds.find(5)  != ds.end() );
  REQUIRE( ds.find(6)  != ds.end() );
  REQUIRE( ds.find(7)  != ds.end() );
  REQUIRE( ds.find(8)  != ds.end() );
  REQUIRE( ds.find(12) != ds.end() );
  REQUIRE( ds.find(13) != ds.end() );
}

TEST_CASE( "DS lifting: benAfter5Adj.txt", "[dominatingSet]" ) {
  Graph *g = read_adjacencylist_from_file("tests/graphs/benAfter5Adj.txt");
  std::set<int> edit{0,1,2,5,6,9,10,11};
  for (auto e : edit)
  {
    g->disable_vertex(e);
  }
  std::set<int> ds = min_DS(g);
  REQUIRE( ds.size() == 3 );
  REQUIRE( ds.find(7)  != ds.end() );
  REQUIRE( ds.find(12) != ds.end() );
  REQUIRE( ds.find(13) != ds.end() );
  std::set<int> ds_lifted = vertex_addition_DS(g, edit, ds);
  REQUIRE( ds_lifted.size() == 7 );
  REQUIRE( ds_lifted.find(0)  != ds_lifted.end() );
  REQUIRE( ds_lifted.find(1)  != ds_lifted.end() );
  REQUIRE( ds_lifted.find(2)  != ds_lifted.end() );
  REQUIRE( ds_lifted.find(5)  != ds_lifted.end() );
  REQUIRE( ds_lifted.find(7)  != ds_lifted.end() );
  REQUIRE( ds_lifted.find(12) != ds_lifted.end() );
  REQUIRE( ds_lifted.find(13) != ds_lifted.end() );
}

// TEST_CASE( "DS all: 10-7-24-graph.txt", "[dominatingSet]" ) {
//   Graph *g = read_adjacencylist_from_file("../../experiments/10-7/10-7-24-graph.txt");
//   std::set<int> edit{2,3,4,5,6,7};
//   for (auto e : edit)
//   {
//     g->disable_vertex(e);
//   }
//   std::set<int> ds = min_DS(g);
//   REQUIRE( ds.size() == 3 );
//   REQUIRE( ds.find(7)  != ds.end() );
//   REQUIRE( ds.find(12) != ds.end() );
//   REQUIRE( ds.find(13) != ds.end() );
//   std::set<int> ds_lifted = vertex_addition_DS(g, edit, ds);
//   REQUIRE( ds_lifted.size() == 7 );
//   REQUIRE( ds_lifted.find(0)  != ds_lifted.end() );
//   REQUIRE( ds_lifted.find(1)  != ds_lifted.end() );
//   REQUIRE( ds_lifted.find(2)  != ds_lifted.end() );
//   REQUIRE( ds_lifted.find(5)  != ds_lifted.end() );
//   REQUIRE( ds_lifted.find(7)  != ds_lifted.end() );
//   REQUIRE( ds_lifted.find(12) != ds_lifted.end() );
//   REQUIRE( ds_lifted.find(13) != ds_lifted.end() );
// }
