#ifndef CELEBRATION_DECORATOR_H_
#define CELEBRATION_DECORATOR_H_

#include "IStrategy.h"
/**
 * @brief A decorator that adds a celebration to the end of a strategy
 *
 * This class is kind of abstract, but constructor is used within Jump and Spin
 * Decorators
 */
class CelebrationDecorator : public IStrategy {
 protected:
  IStrategy* strategy;
  double time;

 public:
  /**
   * @brief Construct a new Celebration Decorator object
   *
   * @param strategy the prior strategy to decorate
   */
  CelebrationDecorator(IStrategy* strategy);
  /**
   * @brief Destroy the Celebration Decorator object
   *
   */
  ~CelebrationDecorator();
  /**
   * @brief Move the entity
   *
   * @param entity the entity to move (and check how to move)
   * @param dt simulation time
   */
  virtual void Move(IEntity* entity, double dt) {}
  /**
   * @brief Check if the strategy is completed
   *
   * @return true
   * @return false
   */
  virtual bool IsCompleted() {}
};

#endif
