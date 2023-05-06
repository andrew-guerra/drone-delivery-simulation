#define _USE_MATH_DEFINES
#include "Horse.h"

#include <stdlib.h>

#include <cmath>
#include <limits>
Horse::Horse(JsonObject& obj) : details(obj) {
  JsonArray pos(obj["position"]);
  position = {pos[0], pos[1], pos[2]};
  JsonArray dir(obj["direction"]);
  direction = {dir[0], dir[1], dir[2]};

  speed = obj["speed"];

  available = true;
}

Horse::~Horse() {
  delete graph;
  delete pathStrategy;
}

void Horse::Update(double dt, std::vector<IEntity*> scheduler) {
  if (available) {
    destination = GetRandomDest();
    available = false;
    pathStrategy = new DijkstraStrategy(position, destination, graph);
    pathStrategy->Move(this, dt);
  } else {
    if (pathStrategy->IsCompleted()) {
      // when strategy completes, update availability
      available = true;
    } else {
      pathStrategy->Move(this, dt);
    }
  }
}

void Horse::Rotate(double angle) {
  Vector3 dirTmp = direction;
  direction.x = dirTmp.x * std::cos(angle) - dirTmp.z * std::sin(angle);
  direction.z = dirTmp.x * std::sin(angle) + dirTmp.z * std::cos(angle);
}

Vector3 Horse::GetRandomDest() {
  Vector3 r(0, 0, 0);
  r.x = rand() % 2900 - 1400;
  r.y = 400;
  r.z = rand() % 1600 - 800;
  return r;
}
