#include "ChargingStationFactory.h"

IEntity* ChargingStationFactory::CreateEntity(JsonObject& entity) {
  std::string type = entity["type"];
  if (type.compare("charging_station") == 0) {
    std::cout << "Charging Station Created" << std::endl;
    return new ChargingStation(entity);
  }
  return nullptr;
}
