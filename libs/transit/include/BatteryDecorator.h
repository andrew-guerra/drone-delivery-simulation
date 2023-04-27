#ifndef BATTERY_DECORATOR_H_
#define BATTERY_DECORATOR_H_
#include "IEntity.h"
#include "../include/routing/astar.h"
#include "../include/routing/depth_first_search.h"
#include "../include/routing/dijkstra.h"
// #include "../include/routing/beeline.h"

// Simulation dimensions x -> [0,1000], z -> [0, 600]
// let's make the drone battery large enough to go from corner to corner before a recharge-> 1166.19 units
// thus 1166.19 / (30 units/sec) = 38.87 seconds before recharge
// This means battery life is just the number of seconds the drone can fly for (or stay idle it's all the same)
const double MAX_BATTERY = 38.87;

class BatteryDecorator : public IEntity{
    protected:
        IEntity *entity;
        
        // This is the number of units of time (seconds?) that the drone can stay in the air
        double battery_life;
        bool charging;

    public:

        BatteryDecorator(IEntity *entity);

        ~BatteryDecorator() {};

        void Update(double dt, std::vector<IEntity*> scheduler);

        bool CanReachDestination(std::vector<IEntity*> scheduler);

        IEntity* GetClosestChargingStation(std::vector<float> location, std::vector<IEntity*> scheduler);

        double GetDistanceToDestination(IEntity *entity);

        Vector3 GetPosition() const;
        Vector3 GetDirection() const;
        Vector3 GetDestination() const;
        JsonObject GetDetails() const;
        float GetSpeed() const;

};

#endif