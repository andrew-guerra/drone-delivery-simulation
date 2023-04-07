#ifndef BEELINE_STRATEGY_H_
#define BEELINE_STRATEGY_H_

#include "IStrategy.h"

/**
 * @brief Will move from one point to another in linear movement direction
 * 
 */
class BeelineStrategy: public IStrategy {
 public:
   /**
    * @brief Construct a new Beeline Strategy object
    * 
    * @param position 
    * @param destination 
    */
    BeelineStrategy(Vector3 position, Vector3 destination);

    /**
     * @brief Destroy the Beeline Strategy object
     * 
     */
    ~BeelineStrategy() {}

    /**
     * @brief Moves the object
     * 
     * @param entity - The IEntity object
     * @param dt - The fraction of 1 second that has passed
     */
    void Move(IEntity* entity, double dt);

    /**
     * @brief Returns true if the strategy is completed
     * 
     * @return true 
     * @return false 
     */
    bool IsCompleted();
 private:
    Vector3 position;
    Vector3 destination;
};

#endif
