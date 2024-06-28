#ifndef vertex_hpp
#define vertex_hpp

#include <iostream>
#include <vector>
#include <set>
#include <utility>

class Vertex
{
  public:
    friend class Graph;
    Vertex(int id, float weight);
    Vertex(int id);
    void add_edge(int v);
    void disable_neighbor(int v);
    void enable_neighbor(int v);
    void remove_edge(int v);
    bool has_edge(int v, bool onlyActive = false);
    int deg(bool onlyActive = false) const;
    std::set<int> get_neighbors(bool onlyActive = false);
    // 0 = black
    // 1 = white
    // 2 = red
    int color = 0;
    /**
     * Disables a vertex so we can consider the subgraph
     * without it while also remembering the full graph
     * Edges with disabled vertices are not counted
     */
    bool active;
    float weight;
    int id;

  protected:
    std::set<int> active_neighbors;
    std::set<int> neighbors;
    int outward_deg;
};

#endif
