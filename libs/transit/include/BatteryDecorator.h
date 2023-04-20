#ifndef BATTERY_DECORATOR_H_
#define BATTERY_DECORATOR_H_
#include "IEntity.h"

class BatteryDecorator{
    protected:
        IEntity *entity;
        double battery_life;

    public:
        BatteryDecorator(IEntity *entity);

        ~BatteryDecorator();

        void Update(double dt);

        double GetStrategyDistance();
};

#endif