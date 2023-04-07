#ifndef ASTAR_STRATEGY_H_
#define ASTAR_STRATEGY_H_

#include "PathStrategy.h"
#include "../../routing/include/routing/astar.h"

/**
 * @brief Will use Astar path finding algorithm to move
 * 
 */
class AstarStrategy: public PathStrategy {
 public:
    /**
     * @brief Construct a new Astar Strategy object
     * 
     * @param position 
     * @param destination 
     * @param graph - The IGraph object
     */
    AstarStrategy(Vector3 position, Vector3 destination,
        const IGraph* graph);
 private:
    const IGraph* graph;
    Vector3 position;
    Vector3 destination;
};

#endif
