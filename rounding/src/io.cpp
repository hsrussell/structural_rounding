#include "../include/io.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

/**
 * Generates a completely random graph
 * Not used currently
 */
void make_graph(int n, int density, std::string filename)
{
  std::ofstream myfile;
  myfile.open (filename);
  for (int i = 0; i < n*density; i++)
  {
    int a = rand() % n;
    int b = rand() % n;
    if (a != b)
    {
      myfile << a << " " << b << "\n";
    }
  }
  myfile.close();
}

/**
 * Reads a graph from a file
 * where each line is in the format "a b c ..."
 * means that graph G contains edge {a,b}, {a,c}, {a,...}
 */
Graph* read_adjacencylist_from_file(std::string filename)
{
  std::ifstream myfile (filename);
  Graph* g = NULL;
  if (myfile.is_open())
  {
    g = new Graph();
    std::string line;
    while (std::getline(myfile, line))
    {
      std::stringstream ss(line);
      int a, b;
      ss >> a;
      g->add_vertex(a);
      while (ss >> b) g->add_edge(a, b);
    }
    myfile.close();
  }
  return g;
}

/**
 * Reads a graph from a file
 * where each line is in the format "a b"
 * means that graph G contains edge {a,b}
 */
 Graph* read_edgelist_from_file(std::string filename)
 {
   std::ifstream myfile (filename);
   Graph* g = NULL;
   if (myfile.is_open())
   {
     int a, b;
     g = new Graph();
     while(myfile >> a >> b) g->add_edge(a, b);
     myfile.close();
   }
   return g;
 }

/**
 * Write the edgelist representation of a graph to a file
 * CANNOT REPRESENT ISOLATED NODES
 */
void write_edgelist_to_file(std::string filename, Graph *g, bool onlyActive)
{
  std::ofstream myfile (filename);
  if (g != NULL)
  {
    std::set<std::pair<int,int>> edges = g->get_edges(onlyActive);
    for (auto i : edges)
    {
      myfile << i.first << " " << i.second << std::endl;
    }
  }
}

/**
 * Write the adjacency list representation of a graph to a file
 */
void write_adjacencylist_to_file(
  std::string filename,
  Graph *g,
  bool onlyActive
)
{
  std::ofstream myfile (filename);
  if (g != NULL)
  {
    //get all active vertices
    std::vector<int> vertices = g->get_vertex_min_deg_order();
    for (auto v : vertices)
    {
      myfile << v << " ";
      std::set<int> neighbors = g->get_neighbors(v, onlyActive);
      for (auto ne : neighbors)
      {
        if (ne > v)
        {
          myfile << ne << " ";
        }
      }
      myfile << std::endl;
    }
  }
}
