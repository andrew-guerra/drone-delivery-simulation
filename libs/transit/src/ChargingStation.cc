#define _USE_MATH_DEFINES
#include "ChargingStation.h"

#include <stdlib.h>

#include <cmath>
#include <limits>

ChargingStation::ChargingStation(JsonObject& obj) : details(obj) {
  JsonArray pos(obj["position"]);
  position = {pos[0], pos[1], pos[2]};
  JsonArray dir(obj["direction"]);
  direction = {dir[0], dir[1], dir[2]};
  speed = obj["speed"];
}

void ChargingStation::Update(double dt, std::vector<IEntity*> scheduler) {
  // std::cout << "updated 1 tick" << std::endl;
}

ChargingStation::~ChargingStation() { delete graph; }
