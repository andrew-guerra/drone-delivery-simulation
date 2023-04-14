#ifndef BATTERY_DECORATOR_H_
#define BATTERY_DECORATOR_H_

class BatteryDecorator{
    protected:
        IEntity *entity;
        double battery_life;

    public:
        BatteryDecorator(IEntity *entity);

        ~BatteryDecorator();

        void Update(double dt);

        float CalculateDistance(vector3 destination, IStrategy *movement_strategy);
}