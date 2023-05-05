#define _USE_MATH_DEFINES
#include "Human.h"

#include <stdlib.h>

#include <cmath>
#include <limits>
Human::Human(JsonObject& obj) : details(obj) {
  JsonArray pos(obj["position"]);
  position = {pos[0], pos[1], pos[2]};
  JsonArray dir(obj["direction"]);
  direction = {dir[0], dir[1], dir[2]};

  speed = obj["speed"];

  available = true;
}
Human::~Human() {
  // Delete dynamically allocated variables
  delete graph;
  delete pathStrategy;
}
void Human::Update(double dt, std::vector<IEntity*> scheduler) {
  if (available) {
    destination = GetRandomDest();
    available = false;
    pathStrategy = new AstarStrategy(position, destination, graph);
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
void Human::Rotate(double angle) {
  Vector3 dirTmp = direction;
  direction.x = dirTmp.x * std::cos(angle) - dirTmp.z * std::sin(angle);
  direction.z = dirTmp.x * std::sin(angle) + dirTmp.z * std::cos(angle);
}
Vector3 Human::GetRandomDest() {
  Vector3 r(0, 0, 0);
  r.x = rand() % 2900 - 1400;
  r.y = 400;
  r.z = rand() % 1600 - 800;
  return r;
}