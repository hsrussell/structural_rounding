#ifndef graph_hpp
#define graph_hpp

#include <iostream>
#include <vector>
#include <set>
#include <utility>
#include "vertex.hpp"

class Graph
{
  public:
    friend class GraphFactory;
    friend class GraphFactoryBI;
    friend class GraphFactoryGMP;
    friend class GraphFactoryDensity;
    friend class GraphFactoryMatrix;
    friend class GraphFactoryMatrixBI;
    friend class GraphFactoryMatrixGMP;
    Graph(int n);
    Graph();
    ~Graph();
    void add_edge(int v1, int v2);
    void add_vertex(int v);
    bool has_edge(int v1, int v2, bool onlyActive = false);
    void remove_edge(int v1, int v2);
    Vertex* get_vertex(int v);
    std::set<std::pair<int,int>> get_edges(bool onlyActive = false);
    void enable_vertex(int v);
    void disable_vertex(int v);
    bool active_vertex(int v);
    std::set<int> get_neighbors(int vertex, bool onlyActive = false, bool ignoreSelf = false);
    std::pair<int,int> get_min_degree_vertex(bool onlyActive = false);
    std::pair<int,int> get_max_degree_vertex(bool onlyActive = false);
    std::vector<int> get_vertex_min_deg_order();
    std::vector<int> get_vertex_min_deg_order_from(std::vector<Vertex *> core);
    std::vector<int> get_vertex_max_deg_order_from(std::vector<Vertex *> core);
    int get_degree(int vertex, bool onlyActive = false, bool ignoreSelf = false);
    std::vector<int> get_vertices(bool onlyActive = true);
    void grow(int n_add);
    void print(bool onlyActive = false);
    int get_n();
    std::vector<Vertex *> vertices;
    void add_vertex(Vertex *v);

  protected:
    int n;
    void incr_outward_deg(int v);
    void decr_outward_deg(int v);
    int get_min_outward_deg();
    int get_vertex_outward_deg(int v);
};

#endif
