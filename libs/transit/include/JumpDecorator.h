#ifndef JUMP_DECORATOR_H_
#define JUMP_DECORATOR_H_

#include "CelebrationDecorator.h"

/**
 * @brief A decorator that adds a jump to the end of a strategy
 * 
 */
class JumpDecorator: public CelebrationDecorator {
    public:
        /**
         * @brief Construct a new Jump Decorator object
         * 
         * @param strategy 
         */
        JumpDecorator(IStrategy* strategy);
        /**
         * @brief Destroy the Jump Decorator object
         * 
         */
        ~JumpDecorator();
        /**
         * @brief Move the entity
         * 
         * @param entity the entity to move (and check how to move)
         * @param dt simulation time
         */
        void Move(IEntity* entity, double dt);

        /**
         * @brief Check if the strategy is completed
         * 
         * @return true 
         * @return false 
         */

        bool IsCompleted();
};

#endif