#include "../../include/graph.hpp"
#include "../../include/vertex.hpp"

#include <algorithm>

struct VertexCompareDeg {
  VertexCompareDeg(Graph *g) : g(g) {}
  bool operator () (Vertex *v1, Vertex *v2) {
    if (v1->deg(true) < v2->deg(true))
    {
      return true;
    }
    if (v1->deg(true) == v2->deg(true))
    {
      return v1->id < v2->id;
    }
    return false;
    // if (g->get_neighbors(v1->id, true).size()
    //   < g->get_neighbors(v2->id, true).size())
    // {
    //   return true;
    // }
    // if (g->get_neighbors(v1->id, true).size()
    //   == g->get_neighbors(v2->id, true).size())
    // {
    //   return v1->id < v2->id;
    // }
    // return false;
  }
  Graph *g;
};

//public
Graph::Graph(int n) : n(n)
{
  for (int i = 0; i < n; i++)
  {
    Vertex *v = new Vertex(i);
    vertices.push_back(v);
  }
}

Graph::Graph() : Graph(0) {}

bool compare(const Vertex *v1, const Vertex *v2)
{
  return v1->deg() < v2->deg();
}

void Graph::add_edge(int v1, int v2)
{
  //allows the graph to grow
  int max = std::max(v1, v2);
  while (max >= n)
  {
    Vertex *vnew = new Vertex(n);
    vertices.push_back(vnew);
    n++;
  }
  vertices[v1]->add_edge(v2);
  vertices[v2]->add_edge(v1);
}

void Graph::add_vertex(int v)
{
  while (v >= n)
  {
    Vertex *vnew = new Vertex(n);
    vertices.push_back(vnew);
    n++;
  }
}

bool Graph::has_edge(int v1, int v2, bool onlyActive)
{
  return (!onlyActive || vertices[v1]->active) &&
    vertices[v1]->has_edge(v2, onlyActive);
}

void Graph::remove_edge(int v1, int v2)
{
  vertices[v1]->remove_edge(v2);
  vertices[v2]->remove_edge(v1);
}

Vertex* Graph::get_vertex(int v)
{
  return vertices[v];
}

std::set<std::pair<int,int>> Graph::get_edges(bool onlyActive)
{
  std::set<std::pair<int, int>> edges;
  for (auto v : vertices)
  {
    std::set<int> neighbors = v->get_neighbors(onlyActive);
    for (auto j : neighbors)
    {
      if (v->id > j) edges.insert(std::make_pair(v->id, j));
    }
  }
  return edges;
}

void Graph::enable_vertex(int v)
{
  vertices[v]->active = true;
  for (auto n : vertices[v]->get_neighbors())
  {
    vertices[n]->enable_neighbor(v);
  }
}

void Graph::disable_vertex(int v)
{
  vertices[v]->active = false;
  // std::cout << "disable: " << v << std::endl;
  for (auto n : vertices[v]->active_neighbors)
  {
    // std::cout << n << " ";
    vertices[n]->disable_neighbor(v);
  }
  // std::cout << std::endl;
}

bool Graph::active_vertex(int v)
{
  return vertices[v]->active;
}

std::set<int> Graph::get_neighbors(int v, bool onlyActive, bool ignoreSelf)
{
  if (onlyActive)
  {
    if (!ignoreSelf && !vertices[v]->active) return std::set<int>();
    return vertices[v]->active_neighbors;
  }
  return vertices[v]->get_neighbors();
}

std::pair<int,int> Graph::get_max_degree_vertex(bool onlyActive)
{
  int max = -1;
  int vertex = -1;
  for (auto v : vertices)
  {
    if (!onlyActive || v->active)
    {
      int deg = v->deg(onlyActive);
      if (deg > max)
      {
        max = deg;
        vertex = v->id;
      }
    }
  }
  return std::make_pair(vertex, max);
}

std::pair<int,int> Graph::get_min_degree_vertex(bool onlyActive)
{
  int min = n;
  int vertex = -1;
  for (auto v : vertices)
  {
    if (!onlyActive || v->active)
    {
      int deg = v->deg(onlyActive);
      if (deg < min)
      {
        min = deg;
        vertex = v->id;
      }
    }
  }
  return std::make_pair(vertex, min);
}

std::vector<int> Graph::get_vertex_min_deg_order()
{
  std::vector<Vertex *> order;
  for (auto v : vertices)
  {
    if (v->active) order.push_back(v);
  }
  std::sort(order.begin(), order.end(), VertexCompareDeg(this));
  std::vector<int> idOrder;
  for (auto v : order) idOrder.push_back(v->id);
  return idOrder;
}

std::vector<int> Graph::get_vertex_min_deg_order_from(std::vector<Vertex *> core)
{
  std::vector<Vertex *> order;
  for (auto i : core)
  {
    if (vertices[i->id]->active)
    {
      order.push_back(i);
    }
  }
  std::sort(order.begin(), order.end(), VertexCompareDeg(this));
  std::vector<int> idOrder;
  for (auto v : order) idOrder.push_back(v->id);
  return idOrder;
}

std::vector<int> Graph::get_vertex_max_deg_order_from(std::vector<Vertex *> core)
{
  std::vector<int> order = get_vertex_min_deg_order_from(core);
  std::reverse(order.begin(), order.end());
  return order;
}

int Graph::get_degree(int v, bool onlyActive, bool ignoreSelf)
{
  return get_neighbors(v, onlyActive, ignoreSelf).size();
}

std::vector<int> Graph::get_vertices(bool onlyActive)
{
  std::vector<int> ret;
  for (auto v : vertices)
  {
    if (v->active || !onlyActive)
    {
      ret.push_back(v->id);
    }
  }
  return ret;
}

void Graph::grow(int n_add)
{
  for (int i = 0; i < n_add; i++)
  {
    Vertex *v = new Vertex(n);
    add_vertex(v);
  }
}

/**
 * Adjacency matrix view of graph
 */
void Graph::print(bool onlyActive)
{
  for (int i = 0; i < n; i++)
  {
    for (int j = 0; j < n; j++)
    {
      std::cout << (has_edge(i, j, onlyActive) ? "1" : " ") << " ";
    }
    std::cout << "\n";
  }
}

int Graph::get_n()
{
  return n;
}

//protected
void Graph::incr_outward_deg(int v)
{
  vertices[v]->outward_deg++;
}

void Graph::decr_outward_deg(int v)
{
  vertices[v]->outward_deg--;
}

int Graph::get_min_outward_deg()
{
  if (vertices.size() == 0) return 0;
  int min = vertices[0]->outward_deg;
  for (int i = 0; i < n; i++)
  {
    min = std::min(min, vertices[i]->outward_deg);
  }
  return min;
}

int Graph::get_vertex_outward_deg(int v)
{
  return vertices[v]->outward_deg;
}

void Graph::add_vertex(Vertex *v)
{
  vertices.push_back(v);
  n++;
}
