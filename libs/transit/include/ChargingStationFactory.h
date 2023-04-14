#ifndef CHARGING_STATION_FACTORY_H_
#define CHARGING_STATION_FACTORY_H_

#include <vector>

#include "ChargingStation.h"
#include "IEntityFactory.h"

class ChargingStationFactory : public IEntityFactory {
    
    public:

        virtual ~ChargingStationFactory() {}

        IEntity* CreateEntity(JsonObject& entity);

};

#endif