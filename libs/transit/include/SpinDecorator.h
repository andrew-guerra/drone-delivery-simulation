#ifndef SPIN_DECORATOR_H_
#define SPIN_DECORATOR_H_

#include "CelebrationDecorator.h"
/**
 * @brief A decorator that adds a spin to the end of a strategy
 *
 */
class SpinDecorator : public CelebrationDecorator {
 public:
  /**
   * @brief Construct a new Spin Decorator object
   *
   * @param strategy the prior strategy to decorate
   */
  SpinDecorator(IStrategy* strategy);
  /**
   * @brief Destroy the Spin Decorator object
   *
   */
  ~SpinDecorator();
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