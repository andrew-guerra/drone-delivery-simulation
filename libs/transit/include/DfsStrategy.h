#ifndef DFS_STRATEGY_H_
#define DFS_STRATEGY_H_

#include "../../routing/include/routing/depth_first_search.h"
#include "PathStrategy.h"

/**
 * @brief Will use DFS path finding algorithm to move
 *
 */
class DfsStrategy : public PathStrategy {
 public:
  /**
   * @brief Construct a new Dfs Strategy object
   *
   * @param position
   * @param destination
   * @param graph
   */
  DfsStrategy(Vector3 position, Vector3 destination, const IGraph* graph);

 private:
  const IGraph* graph;
  Vector3 position;
  Vector3 destination;
};

#endif
