#include "../../include/vertex.hpp"

#include <algorithm>

//public
Vertex::Vertex(int id, float weight) : id(id), weight(weight)
{
  active_neighbors = std::set<int>();
  neighbors = std::set<int>();
  active = true;
  outward_deg = 0;
}

Vertex::Vertex(int id) : Vertex(id, 1) {}

void Vertex::add_edge(int v)
{
  active_neighbors.insert(v);
  neighbors.insert(v);
}

void Vertex::disable_neighbor(int v)
{
  active_neighbors.erase(v);
}

void Vertex::enable_neighbor(int v)
{
  active_neighbors.insert(v);
}

void Vertex::remove_edge(int v)
{
  active_neighbors.erase(v);
  neighbors.erase(v);
}

bool Vertex::has_edge(int v, bool onlyActive)
{
  if (onlyActive)
  {
    return active_neighbors.find(v) != active_neighbors.end();
  }
  return neighbors.find(v) != neighbors.end();
}

int Vertex::deg(bool onlyActive) const
{
  if (onlyActive)
  {
    return active_neighbors.size();
  }
  return neighbors.size();
}

std::set<int> Vertex::get_neighbors(bool onlyActive)
{
  if (onlyActive)
  {
    return active_neighbors;
  }
  return neighbors;
}
