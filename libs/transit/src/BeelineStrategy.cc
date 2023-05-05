
#include "../include/BeelineStrategy.h"

BeelineStrategy::BeelineStrategy(Vector3 position, Vector3 destination) {
  this->position = position;
  this->destination = destination;
}

void BeelineStrategy::Move(IEntity* entity, double dt) {
  this->position = entity->GetPosition();
  // this->destination = entity->GetDestination();
  Vector3 direction = (destination - position).Unit();
  this->position = this->position + (direction * entity->GetSpeed() * dt);
  entity->SetPosition(this->position);
  entity->SetDirection(direction);
}

bool BeelineStrategy::IsCompleted() {
  return position.Distance(destination) < 0.1;
}
