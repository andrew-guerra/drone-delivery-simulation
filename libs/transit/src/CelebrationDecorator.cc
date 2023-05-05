#include "../include/CelebrationDecorator.h"

CelebrationDecorator::CelebrationDecorator(IStrategy* strategy) {
  // Set the initial time to 0
  // Set the strategy
  this->strategy = strategy;
  this->time = 0;
}

CelebrationDecorator::~CelebrationDecorator() {
  // Delete the strategy
  delete strategy;
}
