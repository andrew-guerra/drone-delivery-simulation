#ifndef I_STRATEGY_H_
#define I_STRATEGY_H_

#include "IEntity.h"
// #include "CelebrationDecorator.h"

/**
 * @brief Interface for different pathing strategies
 * 
 */
class IStrategy {
 public:
    /**
     * @brief Moves the Entity
     * 
     * @param entity - The IEntity object
     * @param dt - The fraction of 1 second that has passed
     */
    virtual void Move(IEntity* entity, double dt) = 0;

    /**
     * @brief Returns true is the strategy is completed
     * 
     * @return true 
     * @return false 
     */
    virtual bool IsCompleted() = 0;
};

#endif

