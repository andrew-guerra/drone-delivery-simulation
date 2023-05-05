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

/**
 * @brief Wraps an IEntity and adds battery functionality to it
 * 
 */
class BatteryDecorator : public IEntity {
 public:
 /**
  * @brief Construct a new Battery Decorator object
  * 
  * @param entity Until future updates this will be a drone
  */
  BatteryDecorator(IEntity* entity);

  /**
   * @brief Destroy the Battery Decorator object
   * 
   */
  ~BatteryDecorator(){};

  /**
   * @brief Add each entity in the vector to the simulation 
   * 
   * @param entities A vector of entity objects to be added
   */
  void AddSimEntities(std::vector<IEntity*> entities);

  /**
   * @brief Updates the batteries state and calls entity's update
   * 
   * @param dt Time in seconds since the last update
   * @param scheduler All entities scheduled for delivery
   */
  void Update(double dt, std::vector<IEntity*> scheduler);

  /**
   * @brief Returns true if the drone can make it to the robot, to the robot's destination then to a battery station on it's current charge
   * 
   * @return true 
   * @return false 
   */
  bool CanReachDestination();

  /**
   * @brief Returns true if the drone can make it to the robot, to the robot's destination then to a battery station on it's current charge
   * 
   * @return true 
   * @return false 
   */
  bool CanMakeTrip();

  /**
   * @brief Returns true if the drone can make it location using a beeline strategy
   * 
   * @param location Destination
   * @return true 
   * @return false 
   */
  bool CanReachLocation(Vector3 location);

  /**
   * @brief Returns true if the drone can make it location using a beeline strategy
   * 
   * @param location Destination 
   * @param battery Specified battery life
   * @return true 
   * @return false 
   */
  bool CanReachLocation(Vector3 location, double battery);

  /**
   * @brief Returns true if the entity can reach the charger on current battery life
   * 
   * @param location Used to time the closest charger to this location 
   * @return true 
   * @return false 
   */
  bool CanReachCharger(Vector3 location);

  /**
   * @brief Returns true if the entity can reach the charger on current battery life
   * 
   * @param location Used to time the closest charger to this location 
   * @param battery Specified battery life
   * @return true 
   * @return false 
   */
  bool CanReachCharger(Vector3 location, double battery);

  /**
   * @brief Get the Closest Charging Station object 
   * 
   * @param location Start location
   * @return IEntity* 
   */
  IEntity* GetClosestChargingStation(Vector3 location);

  /**
   * @brief Get the Closest Reachable Charging Station object that the drone can make it to with current battery life
   * 
   * @param location Start location
   * @return IEntity* 
   */
  IEntity* GetClosestReachableChargingStation(Vector3 location);

  /**
   * @brief Get the Distance To Destination object using a path strategy
   * 
   * @param entity An IEntity object 
   * @return double 
   */
  double GetDistanceToDestination(IEntity* entity);

  /**
   * @brief Returns the euclidian distance between two Vector3's
   * 
   * @param start Start location
   * @param des End location 
   * @return double 
   */
  double GetDistanceToLocation(Vector3 start, Vector3 des);

  Vector3 GetPosition() const;
  Vector3 GetDirection() const;
  Vector3 GetDestination() const;
  JsonObject GetDetails() const;
  float GetSpeed() const;
  /**
   * @brief Returns the current batteryLife
   * 
   * @return double 
   */
  double GetBattery() const;

  protected:
   IEntity* entity;
 
   // This is the number seconds that the drone can stay in the air
   double batteryLife;
   double batteryDischargeRateBias;
   bool charging;
   bool travelingToCharger;
   bool deliveringRobot;
   Vector3 closestChargerPosition;
   std::vector<IEntity*> simEntities;
   bool needToCalcDist;
   bool canReach;
   enum options { toCharger, Charging, Default };
   DataCollection* dataCollection;
};

#endif