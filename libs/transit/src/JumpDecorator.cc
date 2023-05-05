#include "../include/JumpDecorator.h"

JumpDecorator::JumpDecorator(IStrategy* strategy)
    : CelebrationDecorator(strategy) {
  time = 0;
}

JumpDecorator::~JumpDecorator() {
  // Delete the strategy
  delete strategy;
}

void JumpDecorator::Move(IEntity* entity, double dt) {
  if (this->strategy->IsCompleted()) {
    // Jump the entity
    while (time < 4) {
      entity->Jump(.25 * dt);
      time += dt;
    }
  } else {
    this->strategy->Move(entity, dt);
  }
}

bool JumpDecorator::IsCompleted() { return time >= 4; }