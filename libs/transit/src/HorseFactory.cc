#include "HorseFactory.h"

IEntity* HorseFactory::CreateEntity(JsonObject& entity) {
  std::string type = entity["type"];
  if (type.compare("horse") == 0) {
    std::cout << "Horse Created" << std::endl;
    return new Horse(entity);
  }
  return nullptr;
}
