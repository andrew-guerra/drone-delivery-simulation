#ifndef BATTERY_DECORATOR_H_
#define BATTERY_DECORATOR_H_
#include "IEntity.h"
#include "../include/routing/astar.h"
#include "../include/routing/depth_first_search.h"
#include "../include/routing/dijkstra.h"

class BatteryDecorator : public IEntity{
    protected:
        IEntity *entity;
        double battery_life;

    public:
        BatteryDecorator(IEntity *entity);

        ~BatteryDecorator();

        void Update(double dt);

        double GetDistanceToDestination();
};

#endif