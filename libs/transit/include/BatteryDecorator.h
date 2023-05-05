#ifndef BATTERY_DECORATOR_H_
#define BATTERY_DECORATOR_H_
#include "../include/routing/astar.h"
#include "../include/routing/depth_first_search.h"
#include "../include/routing/dijkstra.h"
#include "BeelineStrategy.h"
#include "DataCollection.h"
#include "IEntity.h"

// Battery life is just the number of seconds the drone can fly for (or stay
// idle it's all the same)
const double MAX_BATTERY = 70.00;

// Represents how close a battery can be from its minimum charge state
const double BATTERY_DELTA_THREASHOLD = 1.00;

// Represents increase in battery discarge weight based on drone movement
const double BATTERY_MOVEMENT_BIAS = 1.1;

// Represents increase in battery discarge weight based on robot weight
const double BATTERY_ROBOT_LOAD_BIAS = 1.3;

class BatteryDecorator : public IEntity {
 protected:
  IEntity* entity;

  // This is the number of units of time (seconds) that the drone can stay in
  // the air
  double battery_life;
  double battery_discharge_rate_bias;
  bool charging;
  bool travelingToCharge;
  bool deliveringRobot;
  Vector3 closestChargerPosition;
  std::vector<IEntity*> sim_entities;
  bool needToCalcDist;
  bool canReach;
  enum options { toCharger, Charging, Default };
  DataCollection* dataCollection;

 public:
  BatteryDecorator(IEntity* entity);

  ~BatteryDecorator(){};

  void AddSimEntities(std::vector<IEntity*> entities);

  void Update(double dt, std::vector<IEntity*> scheduler);

  bool CanReachDestination();

  bool CanMakeTrip();

  bool CanReachLocation(Vector3 location);

  bool CanReachLocation(Vector3 location, double battery);

  bool CanReachCharger(Vector3 location);

  bool CanReachCharger(Vector3 location, double battery);

  IEntity* GetClosestChargingStation(Vector3 location);

  IEntity* GetClosestReachableChargingStation(Vector3 location);

  double GetDistanceToDestination(IEntity* entity);

  double GetDistanceToLocation(Vector3 start, Vector3 des);

  Vector3 GetPosition() const;
  Vector3 GetDirection() const;
  Vector3 GetDestination() const;
  JsonObject GetDetails() const;
  float GetSpeed() const;
  double GetBattery() const;
};

#endif