#include "../include/DataCollection.h"

#include "../include/BatteryDecorator.h"

// Define the instance pointer
DataCollection* DataCollection::instancePtr = nullptr;

DataCollection::~DataCollection() {
  std::cout << "Destroying data collection" << std::endl;
  std::map<std::string, DroneData*>::iterator it;
  std::map<std::string, RobotData*>::iterator it2;

  for (it = this->droneData.begin(); it != this->droneData.end(); it++) {
    delete it->second;
  }

  for (it2 = this->robotData.begin(); it2 != this->robotData.end(); it2++) {
    delete it2->second;
  }
}

DataCollection* DataCollection::GetInstance() {
  if (DataCollection::instancePtr == NULL) {
    DataCollection::instancePtr = new DataCollection();
  }
  return DataCollection::instancePtr;
}

DataCollection::DataCollection() {
  this->totalElapsedTime = 0.0;
  this->observer = GUIDataObserver::GetInstance();
}

void DataCollection::addDrone(IEntity* drone) {
  DroneData* newDroneData = new DroneData();
  JsonObject details = drone->GetDetails();
  std::string name = details["name"];

  // Map the drone's name to the new DroneData object
  this->droneData[name] = newDroneData;
}

void DataCollection::addRobot(IEntity* robot) {
  RobotData* newRobotData = new RobotData();
  JsonObject details = robot->GetDetails();
  std::string name = details["name"];

  // Map the robot's name to the new RobotData object
  this->robotData[name] = newRobotData;
}

void DataCollection::addStationStop(IEntity* drone) {
  JsonObject details = drone->GetDetails();
  std::string name = details["name"];

  this->droneData[name]->numChargingStationStops++;
}

void DataCollection::addDelivery(IEntity* drone) {
  JsonObject details = drone->GetDetails();
  std::string name = details["name"];

  this->droneData[name]->numDeliveries++;
}

void DataCollection::addNewDeliveryTime(IEntity* drone) {
  // add the new delivery time in terms of global simulation time
  JsonObject details = drone->GetDetails();
  std::string name = details["name"];

  this->droneData[name]->deliveryTimes.push_back(this->totalElapsedTime);
}

void DataCollection::addNewPositionDrone(IEntity* drone, Vector3 pos) {
  JsonObject details = drone->GetDetails();
  std::string name = details["name"];
  Vector3 previous_position;
  if (this->droneData[name]->positions.empty()) {
    previous_position = drone->GetPosition();
  } else {
    // The last element of the vector
    previous_position =this->droneData[name]->positions.back();  
  }

  // Add the new position to the vector
  this->droneData[name]->positions.push_back(pos);  

  // Add the (linear) distance between the two positions
  this->droneData[name]->distanceTraveled += previous_position.Distance(pos);  
}

void DataCollection::addNewPositionRobot(IEntity* robot, Vector3 pos) {
  JsonObject details = robot->GetDetails();
  std::string name = details["name"];

  Vector3 previous_position;
  if (this->robotData[name]->positions.empty()) {
    previous_position = robot->GetPosition();
  } else {
     // The last element of the vector
    previous_position =this->robotData[name]->positions.back();
  }
  this->robotData[name]->positions.push_back(pos);  // Add the new position to the vector

  // Add the (linear) distance between the two positions
  this->robotData[name]->distanceTraveled += previous_position.Distance(pos);  
}

void DataCollection::updateBatteryDrone(IEntity* drone, double battery) {
  JsonObject details = drone->GetDetails();
  std::string name = details["name"];

  this->droneData[name]->battery = battery;
}

// updates the total time of the simulation
void DataCollection::updateSimTime(double dt) {
  this->totalElapsedTime += dt;
  this->Notify();
}

JsonObject DataCollection::generateWebJSON() {
  JsonObject obj = JsonObject();
  obj["time-elapsed"] = totalElapsedTime;
  
  for (auto it = this->droneData.begin(); it != this->droneData.end(); ++it) {
    JsonObject droneJson = JsonObject();

    DroneData* droneData = this->droneData.at(it->first);
    droneJson["name"] = it->first;
    droneJson["battery-percentage"] = (droneData->battery / MAX_BATTERY) * 100;
    droneJson["distance"] = droneData->distanceTraveled;
    droneJson["deliveries"] = droneData->numDeliveries;

    obj[it->first] = droneJson;
  }

  return obj;
}

void DataCollection::Notify() { this->observer->SetJSON(generateWebJSON()); }

void DataCollection::generateJSON() {
  std::cout << "Generating JSON file\n";
  
  std::map<std::string, DroneData*>::iterator it;
  std::map<std::string, RobotData*>::iterator it2;
  int i = 1;
  int size = 0;

  // Creates a new timestamp of the current date and time to use as the file
  // name
  std::time_t now = std::time(nullptr);
  char timestamp[40];
  std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d::%H-%M-%S",
                std::localtime(&now));
  std::string stamp = std::string(timestamp);
  stamp += ".json";

  std::cout << stamp << std::endl;
  std::ofstream outfile(stamp);
  if (!outfile) {
    std::cout << "Failed to open json file for output" << std::endl;
    exit(1);
  }

  outfile << "{\n\t";
  outfile << "\"simulation_time\": " << totalElapsedTime << ",\n\t";
  outfile << "\"drones\": {";
  for (it = this->droneData.begin(); it != this->droneData.end(); it++) {
    outfile << "\n\t\t\"drone_" << i << "\": {\n\t\t\t";
    outfile << "\"distanceTraveled\": " << it->second->distanceTraveled
            << ",\n\t\t\t";
    outfile << "\"number_of_charging_station_visits\": "
            << it->second->numChargingStationStops << ",\n\t\t\t";
    outfile << "\"number_of_deliveries\": " << it->second->numDeliveries
            << ",\n\t\t\t";
    outfile << "\"deliveryTimes\": [";
    size = it->second->deliveryTimes.size();
    for (int j = 0; j < size; j++) {
      outfile << it->second->deliveryTimes[i];
      if (j != size - 1) {
        outfile << ", ";
      }
    }
    outfile << "],\n\t\t\t";
    outfile << "\"positions\": [\n\t\t\t\t";
    size = it->second->positions.size();
    for (int j = 0; j < size; j++) {
      outfile << "[" << it->second->positions[j].x << ", "
              << it->second->positions[j].y << ", "
              << it->second->positions[j].z << "]";
      if (j != size - 1) {
        outfile << ",\n\t\t\t\t";
      }
    }
    outfile << "\n\t\t\t]";
    if (std::next(it) == droneData.end()) {
      outfile << "\n\t\t}";
    } else {
      outfile << "\n\t\t},";  // close "drone_i": {
    }
    i++;
  }
  outfile << "\n\t},";  // close "drones": {
  // Begin outputting robot data
  outfile << "\n\t\"robots\": {\n\t\t";
  i = 1;
  for (it2 = this->robotData.begin(); it2 != this->robotData.end(); it2++) {
    outfile << "\"robot_" << i << "\": {\n\t\t\t";
    outfile << "\"distanceTraveled\": " << it2->second->distanceTraveled
            << ",\n\t\t\t";
    outfile << "\"positions\": [\n\t\t\t\t";
    size = it2->second->positions.size();
    for (int j = 0; j < size; j++) {
      outfile << "[" << it2->second->positions[j].x << ", "
              << it2->second->positions[j].y << ", "
              << it2->second->positions[j].z << "]";
      if (j != size - 1) {
        outfile << ",\n\t\t\t\t";
      }
    }
    outfile << "\n\t\t\t]";
    if (std::next(it2) == robotData.end()) {
      outfile << "\n\t\t}";
    } else {
      outfile << "\n\t\t},";  // close "robot_i": {
    }
    i++;
  }

  outfile << "\n\t}";  // close "robots": {
  outfile << "\n}";    // close json data
  outfile.close();
}