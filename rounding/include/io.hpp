#ifndef io_hpp
#define io_hpp

#include "graph.hpp"

#include <iostream>
#include <vector>

void make_graph(int n, int density, std::string filename);

Graph* read_edgelist_from_file(std::string filename);

Graph* read_adjacencylist_from_file(std::string filename);

void write_edgelist_to_file(std::string filename, Graph *g, bool onlyActive);

void write_adjacencylist_to_file(std::string filename, Graph *g, bool onlyActive);

#endif
