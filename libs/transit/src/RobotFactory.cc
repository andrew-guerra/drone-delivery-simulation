#include "RobotFactory.h"

IEntity* RobotFactory::CreateEntity(JsonObject& entity) {
  std::string type = entity["type"];
  if (type.compare("robot") == 0) {
    std::cout << "Robot Created" << std::endl;
    IEntity* nRobot = new Robot(entity);
    DataCollection* dataCollection = DataCollection::GetInstance();
    dataCollection->addRobot(nRobot);
    dataCollection->addNewPositionRobot(nRobot, nRobot->GetPosition());
    return nRobot;
  }
  return nullptr;
}
