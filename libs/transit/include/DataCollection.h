#ifndef DATA_COLLECTION_H_
#define DATA_COLLECTION_H_

#include <ctime>
#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "GUIDataObserver.h"
#include "IEntity.h"
#include "math/vector3.h"

typedef struct DroneData {
  DroneData() {
    numDeliveries = 0;
    numChargingStationStops = 0;
    distanceTraveled = 0.0;
    positions = std::vector<Vector3>();
    deliveryTimes = std::vector<double>();
  }

  std::vector<Vector3> positions;
  std::vector<double> deliveryTimes;
  int numDeliveries;
  int numChargingStationStops;
  double distanceTraveled;
  double battery;
} DroneData;

typedef struct RobotData {
  RobotData() { distanceTraveled = 0.0; }

  double distanceTraveled;
  std::vector<Vector3> positions;
} RobotData;

/**
 * @brief A Singleton that collects data from drones and robots
 *
 */
class DataCollection {
 public:
  /**
   * @brief Destroy the Data Collection object
   *
   */
  ~DataCollection();

  /**
   * Singletons should not be assignable.
   */
  void operator=(const DataCollection&) = delete;

  /**
   * @brief Get the Instance object (on first run it creates it. On subsequent
   * runs it returns the existing static object)
   *
   * @return DataCollection*
   */
  static DataCollection* GetInstance();

  /**
   * @brief track a new drone's data
   *
   * @param drone a pointer to a drone object
   */
  void addDrone(IEntity* drone);

  /**
   * @brief track a new robot's data
   *
   * @param robot a pointer to a new robot object
   */
  void addRobot(IEntity* robot);

  /**
   * @brief update number of charge station stops
   *
   * @param drone pointer to a drone object
   */
  void addStationStop(IEntity* drone);

  /**
   * @brief update distance traveled for drone
   *
   * @param drone pointer to a drone object
   */
  void updateDistanceDrone(IEntity* drone);

  /**
   * @brief updates drone battery data
   *
   * @param drone pointer to a drone object
   * @param battery battery for drone
   */
  void updateBatteryDrone(IEntity* drone, double battery);

  /**
   * @brief update number of passengers dropped off
   *
   * @param drone pointer to a drone object
   */
  void addDelivery(IEntity* drone);

  /**
   * @brief add a new time for delivery (uses current simulation time)
   *
   * @param drone pointer to a drone object
   */
  void addNewDeliveryTime(IEntity* drone);

  /**
   * @brief add a new position to the vector of previous positions
   *
   * @param drone pointer to a drone object
   * @param pos The position to be added (Usually the drone's new location)
   */
  void addNewPositionDrone(IEntity* drone, Vector3 pos);

  /**
   * @brief update distance traveled for robot
   *
   * @param robot pointer to a robot object
   */
  void updateDistanceRobot(IEntity* robot);

  /**
   * @brief add a new position to the vector of previous positions
   *
   * @param robot pointer to a robot object
   * @param pos
   */
  void addNewPositionRobot(IEntity* robot, Vector3 pos);

  /**
   * @brief update the total simulation
   *
   * @param dt amount to adjust total time by
   */
  void updateSimTime(double dt);

  /**
   * @brief When the state of the class changes, update the Json object of the observer
   *
   */
  void Notify();

  /**
   * @brief Create a JsonObject with the latest statistics from the simulation for the observer
   *
   * @return JsonObject
   */
  JsonObject generateWebJSON();

  /**
   * @brief creates a csv file with all of the simulation's current data
   *
   */
  void generateJSON();

 private:
  DataCollection();
  static DataCollection* instancePtr;
  GUIDataObserver* observer;
  double totalElapsedTime;
  std::map<std::string, DroneData*> droneData;
  std::map<std::string, RobotData*> robotData;
};

#endif
