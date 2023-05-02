#include "DroneFactory.h"

IEntity* DroneFactory::CreateEntity(JsonObject& entity) {
  std::string type = entity["type"];
  if (type.compare("drone") == 0) {
    std::cout << "Drone Created" << std::endl;
    IEntity *nDrone = new Drone(entity);
    nDrone = new BatteryDecorator(nDrone);
    return nDrone;
  }
  return nullptr;
}
