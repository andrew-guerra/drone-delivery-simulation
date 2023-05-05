#ifndef CHARGING_STATION_FACTORY_H_
#define CHARGING_STATION_FACTORY_H_

#include <vector>

#include "ChargingStation.h"
#include "IEntityFactory.h"

/**
 * @brief Charging Station Factory: creates the charging station objects
 *
 */
class ChargingStationFactory : public IEntityFactory {
 public:
  /**
   * @brief Destroy the Charging Station Factory object
   *
   */
  virtual ~ChargingStationFactory() {}
  /**
   * @brief Create a Entity object
   *
   * @param entity
   * @return the IEntity* object of the Charging Station
   */
  IEntity* CreateEntity(JsonObject& entity);
};

#endif