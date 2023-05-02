#include "DataCollection.h"

DataCollection* DataCollection::GetInstance() {
    if (instancePtr == NULL) {
        instancePtr = new DataCollection();
    } 
    return instancePtr;
}

DataCollection::DataCollection() {
    total_elapsed_time = 0.0;
    
}

void DataCollection::addDrone(IEntity* drone){
    DroneData newDrone = DroneData();
    JsonObject details = drone->GetDetails();
    std::string name = details["name"];

    // Map the drone's name to the new DroneData object
    drone_data[name] = newDrone;
}

void DataCollection::addRobot(IEntity* robot){
    DroneData newRobot = RobotData();
    JsonObject details = robot->GetDetails();
    std::string name = details["name"];

    // Map the robot's name to the new RobotData object
    robot_data[name] = newRobot;
}


void DataCollection::generateCSV();

// update number of charge station stops
void DataCollection::addStationStop(IEntity *drone);
// update distance traveled for drone
void DataCollection::updateDistanceDrone(IEntity* drone);
// update number of passengers picked up
void DataCollection::addPassenger(IEntity* drone);
// add new position
void DataCollection::addNewPositionDrone(IEntity* drone, Vector3 pos);
// add a new time for delivery (uses current simulation time)
void DataCollection::addNewDeliveryTime(IEntity* drone);

// update distance traveled for robot
void DataCollection::updateDistanceRobot(IEntity* robot);
// add new position
void DataCollection::addNewPositionRobot(IEntity* robot, Vector3 pos);

void DataCollection::updateSimTime(double dt);
