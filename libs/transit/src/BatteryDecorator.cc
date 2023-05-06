#include "../include/BatteryDecorator.h"

BatteryDecorator::BatteryDecorator(IEntity* entity) {
  this->entity = entity;
  this->batteryLife = MAX_BATTERY;
  this->batteryDischargeRateBias = 0;
  this->charging = false;
  this->travelingToCharger = false;
  this->needToCalcDist = true;
  this->canReach = true;
  this->dataCollection = DataCollection::GetInstance();
}

void BatteryDecorator::AddSimEntities(std::vector<IEntity*> entities) {
  this->simEntities = entities;
}

double BatteryDecorator::GetDistanceToDestination(IEntity* entity) {
  std::string strategyName = entity->GetStrategyName();
  std::vector<std::vector<float> > path;
  std::vector<float> pos;
  std::vector<float> des;
  EuclideanDistance eucDist;
  double distance = 0;

  pos.push_back(entity->GetPosition().x);
  pos.push_back(entity->GetPosition().y);
  pos.push_back(entity->GetPosition().z);
  des.push_back(entity->GetDestination().x);
  des.push_back(entity->GetDestination().y);
  des.push_back(entity->GetDestination().z);

  // get path based on routing strategy
  if (strategyName.compare("astar") == 0) {
    path = entity->GetGraph()->GetPath(pos, des, AStar::Default());

  } else if (strategyName.compare("dfs") == 0) {
    path = entity->GetGraph()->GetPath(pos, des, DepthFirstSearch::Default());

  } else if (strategyName.compare("dijkstra") == 0) {
    path = entity->GetGraph()->GetPath(pos, des, Dijkstra::Default());
  }

  // calculate the distance between every point
  for (int i = 0; i < path.size() - 2; i++) {
    distance += eucDist.Calculate(path[i], path[i + 1]);
  }

  return distance;
}

double BatteryDecorator::GetDistanceToLocation(Vector3 start, Vector3 dest) {
  EuclideanDistance eucDist;

  std::vector<float> startVec;
  startVec.push_back(start.x);
  startVec.push_back(start.y);
  startVec.push_back(start.z);

  std::vector<float> destVec;
  destVec.push_back(dest.x);
  destVec.push_back(dest.y);
  destVec.push_back(dest.z);

  return eucDist.Calculate(startVec, destVec);
}

IEntity* BatteryDecorator::GetClosestChargingStation(Vector3 location) {
  IEntity* closest = NULL;
  EuclideanDistance eucDist;
  double minDist = 9999999999999999.0;
  std::string chargerName;

  for (int i = 0; i < simEntities.size(); i++) {
    // Get the entity's details
    JsonObject details = simEntities[i]->GetDetails();
    std::string name = details["type"];

    // If the entity is a charging station then get its position and compare
    if (name.compare("charging_station") == 0) {
      double currDist =
          GetDistanceToLocation(location, simEntities[i]->GetPosition());

      if (currDist < minDist) {
        chargerName = static_cast<std::string>(details["name"]);
        minDist = currDist;
        closest = simEntities[i];
      }
    }
  }

  return closest;
}

IEntity* BatteryDecorator::GetClosestReachableChargingStation(
    Vector3 location) {
  IEntity* closest = NULL;
  EuclideanDistance eucDist;
  double minDist = 9999999999999999.0;

  for (int i = 0; i < simEntities.size(); i++) {
    // Get the entity's details
    JsonObject details = simEntities[i]->GetDetails();
    std::string name = details["type"];

    // If the entity is a charging station then get its position and compare
    if (name.compare("charging_station") == 0) {
      double currDist =
          GetDistanceToLocation(location, simEntities[i]->GetPosition());

      if (CanReachLocation(simEntities[i]->GetPosition())
                                    && currDist < minDist) {
        minDist = currDist;
        closest = simEntities[i];
      }
    }
  }

  return closest;
}

// Returns true if the drone can make it to the robot, to the robot's
// destination then to a battery station on it's current charge
bool BatteryDecorator::CanReachDestination() {
  // Get the distance from the entity's position to its destination
  double distance;
  if (deliveringRobot) {
    distance = GetDistanceToDestination(entity->GetEntity());

    // Add in distance from the drone to the robot
    distance +=
        entity->GetPosition().Distance(entity->GetEntity()->GetPosition());

    // get the location of the closest charging station
    IEntity* closestCharger =
        GetClosestChargingStation(entity->GetEntity()->GetDestination());
    if (closestCharger == NULL) {
      return false;
    }

    // Add in the distance from the robot's location to the closest charger
    Vector3 dest = entity->GetEntity()->GetDestination();
    Vector3 pos = closestCharger->GetPosition();

    distance += pos.Distance(dest);
  } else {
    distance = entity->GetPosition().Distance(entity->GetDestination());
  }

  // drone speed = 30 units/time unit
  // Calculate the time it will take to reach the destination.
  double time = distance / entity->GetSpeed();
  double biasedBatteryLife = batteryLife - BATTERY_DELTA_THREASHOLD;
  return time * batteryDischargeRateBias < biasedBatteryLife;
}

bool BatteryDecorator::CanMakeTrip() {
  double distance = 0;

  if (!CanReachLocation(entity->GetDestination())) {
    IEntity* charger =
        GetClosestReachableChargingStation(entity->GetDestination());

    // distance to charging station closest to robot
    distance += GetDistanceToLocation(GetPosition(), charger->GetPosition());

    // distance from charing station to robot
    distance += GetDistanceToLocation(charger->GetPosition(), GetDestination());
  } else {
    // distance to robot
    distance += GetDistanceToDestination(entity->GetEntity());
  }

  // distance of robot trip
  distance += GetDistanceToDestination(entity->GetEntity()->GetEntity());

  // distance from robot destination to nearest charger
  distance +=
      GetDistanceToLocation(entity->GetEntity()->GetEntity()->GetPosition(),
                            entity->GetEntity()->GetEntity()->GetDestination());

  // drone speed = 30 units/time unit
  // Calculate the time it will take to reach the destination.
  double time = distance / entity->GetSpeed();
  double biasedBatteryLife = batteryLife - BATTERY_DELTA_THREASHOLD;
  return time * batteryDischargeRateBias < biasedBatteryLife;
}

bool BatteryDecorator::CanReachCharger(Vector3 location) {
  // get the location of the closest charging station
  IEntity* closestCharger = GetClosestChargingStation(location);
  double distance = location.Distance(closestCharger->GetPosition());

  double time = (distance) / entity->GetSpeed();

  return CanReachLocation(closestCharger->GetPosition(), batteryLife - time);
}

bool BatteryDecorator::CanReachCharger(Vector3 location, double battery) {
  // get the location of the closest charging station
  IEntity* closestCharger = GetClosestChargingStation(location);
  double distance = location.Distance(closestCharger->GetPosition());
  double time = (distance) / entity->GetSpeed();

  return CanReachLocation(closestCharger->GetPosition(), battery - time);
}

// Returns true if the drone can make it location using a beeline strategy
bool BatteryDecorator::CanReachLocation(Vector3 location) {
  // Get the distance from the entity's position to the location
  double distance = entity->GetPosition().Distance(location);

  // drone speed = 30 units/time unit
  // Calculate the time it will take to reach the destination.
  double time = distance / entity->GetSpeed();
  double biasedBatteryLife = batteryLife - BATTERY_DELTA_THREASHOLD;
  return time * batteryDischargeRateBias < biasedBatteryLife;
}

// Returns true if the drone can make it location using a beeline strategy
bool BatteryDecorator::CanReachLocation(Vector3 location, double battery) {
  // Get the distance from the entity's position to the location
  double distance = entity->GetPosition().Distance(location);

  // drone speed = 30 units/time unit
  // Calculate the time it will take to reach the destination.
  double time = distance / entity->GetSpeed();

  return time * batteryDischargeRateBias < battery - BATTERY_DELTA_THREASHOLD;
}

void BatteryDecorator::Update(double dt, std::vector<IEntity*> scheduler) {
  options opts;
  batteryDischargeRateBias = 1;

  if (charging) {
    opts = Charging;
  } else if (travelingToCharger) {
    opts = toCharger;
  } else if (!entity->GetAvailability()) {
    if (needToCalcDist) {
      deliveringRobot = true;
      double combinedBias = BATTERY_MOVEMENT_BIAS * BATTERY_ROBOT_LOAD_BIAS;
      batteryDischargeRateBias = combinedBias;
      canReach = CanReachDestination();
      needToCalcDist = false;

      batteryDischargeRateBias = 1;
    }

    if (canReach) {
      deliveringRobot = true;
      opts = Default;
    } else {
      double combinedBias = BATTERY_MOVEMENT_BIAS * BATTERY_ROBOT_LOAD_BIAS;
      batteryDischargeRateBias = combinedBias;
      // find the closest charging station
      IEntity* charger = GetClosestChargingStation(GetPosition());

      // set the new destination to be the charger
      closestChargerPosition = charger->GetPosition();

      deliveringRobot = true;
      travelingToCharger = true;
      opts = toCharger;
    }

  } else {
    // Drone avail and dormant
    deliveringRobot = false;
    needToCalcDist = true;

    // find the closest charging station
    IEntity* charger = GetClosestChargingStation(GetPosition());

    batteryDischargeRateBias = BATTERY_MOVEMENT_BIAS;

    if (!CanReachLocation(charger->GetPosition())) {
      travelingToCharger = true;

      // set the new destination to be the charger
      closestChargerPosition = charger->GetPosition();
      opts = toCharger;
    } else {
      opts = Default;
    }
  }

  // is moving
  if (travelingToCharger || deliveringRobot) {
    batteryDischargeRateBias *= BATTERY_MOVEMENT_BIAS;
  }

  // is carrying robot
  if (deliveringRobot) {
    batteryDischargeRateBias *= BATTERY_ROBOT_LOAD_BIAS;
  }

  switch (opts) {
    case Charging:
      if (batteryLife >= MAX_BATTERY) {
        batteryLife = MAX_BATTERY;
        charging = false;
        needToCalcDist = true;

        // Add in one stop at a charging station after the charging completes
        dataCollection->addStationStop(entity);
      }
      break;

    case toCharger:
      if (GetPosition().Distance(closestChargerPosition) < 1) {
        // here we have reached the charging station
        charging = true;
        travelingToCharger = false;
      } else {
        IStrategy* pathStrategy = new BeelineStrategy(GetPosition(),
                                                      closestChargerPosition);
        pathStrategy->Move(entity, dt);

        // Add a new point to the drone's position
        dataCollection->addNewPositionDrone(entity, entity->GetPosition());
      }
      break;

    case Default:
      entity->Update(dt, scheduler);
      break;
  }

  if (charging == true) {
    this->batteryLife += 4 * dt;
  } else {
    this->batteryLife -= dt * batteryDischargeRateBias;
  }

  dataCollection->updateBatteryDrone(entity, this->batteryLife);
}

Vector3 BatteryDecorator::GetPosition() const { return entity->GetPosition(); }

Vector3 BatteryDecorator::GetDirection() const {
  return entity->GetDirection();
}

Vector3 BatteryDecorator::GetDestination() const {
  return entity->GetDestination();
}

JsonObject BatteryDecorator::GetDetails() const { return entity->GetDetails(); }

float BatteryDecorator::GetSpeed() const { return entity->GetSpeed(); }

double BatteryDecorator::GetBattery() const { return this->batteryLife; }
