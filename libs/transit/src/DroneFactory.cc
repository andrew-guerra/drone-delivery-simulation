#include "DroneFactory.h"

IEntity* DroneFactory::CreateEntity(JsonObject& entity) {
  std::string type = entity["type"];
  if (type.compare("drone") == 0) {
    std::cout << "Drone Created" << std::endl;
    IEntity* nDrone = new Drone(entity);
    nDrone = new BatteryDecorator(nDrone);
    DataCollection* dataCollection = DataCollection::GetInstance();
    dataCollection->addDrone(nDrone);
    dataCollection->addNewPositionDrone(nDrone, nDrone->GetPosition());
    return nDrone;
  }
  return nullptr;
}
