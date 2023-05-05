
#include "../include/DijkstraStrategy.h"

DijkstraStrategy::DijkstraStrategy(Vector3 position, Vector3 destination,
                                   const IGraph* graph)
    : PathStrategy() {
  this->graph = graph;
  this->position = position;
  this->destination = destination;
  this->index = 0;
  std::vector<float> src = {position.x, position.y, position.z};
  std::vector<float> dest = {destination.x, destination.y, destination.z};
  this->path = graph->GetPath(src, dest, Dijkstra::Default());
}
