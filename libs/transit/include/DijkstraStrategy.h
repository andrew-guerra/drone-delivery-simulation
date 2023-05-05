#ifndef DIJKSTRA_STRATEGY_H_
#define DIJKSTRA_STRATEGY_H_

#include "../../routing/include/routing/dijkstra.h"
#include "PathStrategy.h"

/**
 * @brief Will use Dijkstra path finding algorithm to move
 *
 */
class DijkstraStrategy : public PathStrategy {
 public:
  /**
   * @brief Construct a new Dijkstra Strategy object
   *
   * @param position
   * @param destination
   * @param graph - The IGraph object
   */
  DijkstraStrategy(Vector3 position, Vector3 destination, const IGraph* graph);

 private:
  const IGraph* graph;
  Vector3 position;
  Vector3 destination;
};

#endif
