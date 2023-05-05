#include "../include/DataCollection.h"

#include "../include/BatteryDecorator.h"

// Define the instance pointer
DataCollection* DataCollection::instancePtr = nullptr;

DataCollection::~DataCollection() {
  std::cout << "Destroying data collection" << std::endl;
  std::map<std::string, DroneData*>::iterator it;
  std::map<std::string, RobotData*>::iterator it2;

  for (it = this->drone_data.begin(); it != this->drone_data.end(); it++) {
    delete it->second;
  }

  for (it2 = this->robot_data.begin(); it2 != this->robot_data.end(); it2++) {
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
  this->total_elapsed_time = 0.0;
  this->observer = GUIDataObserver::GetInstance();
}

void DataCollection::addDrone(IEntity* drone) {
  std::cout << "Added Drone to data collection: ";
  DroneData* new_drone_data = new DroneData();
  JsonObject details = drone->GetDetails();
  std::string name = details["name"];
  // std::cout << "name: " << name << std::endl;

  // Map the drone's name to the new DroneData object
  this->drone_data[name] = new_drone_data;
}

void DataCollection::addRobot(IEntity* robot) {
  std::cout << "Added Robot to data collection\n";
  RobotData* new_robot_data = new RobotData();
  JsonObject details = robot->GetDetails();
  std::string name = details["name"];

  // Map the robot's name to the new RobotData object
  this->robot_data[name] = new_robot_data;
}

void DataCollection::addStationStop(IEntity* drone) {
  std::cout << "Added station stop to data collection\n";
  JsonObject details = drone->GetDetails();
  std::string name = details["name"];

  this->drone_data[name]->num_charging_station_stops++;
}

void DataCollection::addDelivery(IEntity* drone) {
  std::cout << "Added new delivery to data collection\n";
  JsonObject details = drone->GetDetails();
  std::string name = details["name"];

  this->drone_data[name]->num_deliveries++;
}

void DataCollection::addNewDeliveryTime(IEntity* drone) {
  std::cout << "Added new delivery time to data collection\n";
  // add the new delivery time in terms of global simulation time
  JsonObject details = drone->GetDetails();
  std::string name = details["name"];

  this->drone_data[name]->delivery_times.push_back(this->total_elapsed_time);
}

void DataCollection::addNewPositionDrone(IEntity* drone, Vector3 pos) {
  // std::cout << "Adding new drone position to data collection\n";
  JsonObject details = drone->GetDetails();
  std::string name = details["name"];
  Vector3 previous_position;
  if (this->drone_data[name]->positions.empty()) {
    previous_position = drone->GetPosition();
  } else {
    previous_position =
        this->drone_data[name]
            ->positions.back();  // The last element of the vector
  }

  this->drone_data[name]->positions.push_back(
      pos);  // Add the new position to the vector

  this->drone_data[name]->distance_traveled += previous_position.Distance(
      pos);  // Add the (linear) distance between the two positions
             // std::cout << "fin\n";
}

void DataCollection::addNewPositionRobot(IEntity* robot, Vector3 pos) {
  // std::cout << "Adding new robot position to data collection\n";
  JsonObject details = robot->GetDetails();
  std::string name = details["name"];
  // std::cout << name << std::endl;

  Vector3 previous_position;
  if (this->robot_data[name]->positions.empty()) {
    previous_position = robot->GetPosition();
  } else {
    previous_position =
        this->robot_data[name]
            ->positions.back();  // The last element of the vector
  }
  this->robot_data[name]->positions.push_back(
      pos);  // Add the new position to the vector

  this->robot_data[name]->distance_traveled += previous_position.Distance(
      pos);  // Add the (linear) distance between the two positions}
             // std::cout << "fin\n";
}

void DataCollection::updateBatteryDrone(IEntity* drone, double battery) {
  JsonObject details = drone->GetDetails();
  std::string name = details["name"];

  this->drone_data[name]->battery = battery;
}

// updates the total time of the simulation
void DataCollection::updateSimTime(double dt) {
  this->total_elapsed_time += dt;
  this->Notify();
}

JsonObject DataCollection::generateWebJSON() {
  JsonObject obj = JsonObject();

  for (auto it = this->drone_data.begin(); it != this->drone_data.end(); ++it) {
    JsonObject droneJson = JsonObject();

    DroneData* droneData = this->drone_data.at(it->first);
    droneJson["name"] = it->first;
    droneJson["battery-percentage"] = (droneData->battery / MAX_BATTERY) * 100;
    droneJson["distance"] = droneData->distance_traveled;
    droneJson["deliveries"] = droneData->num_deliveries;

    obj[it->first] = droneJson;
  }
  /*DroneData* drone1Data = this->drone_data.at("Drone-1");
  drone1["name"] = "Drone-1";
  drone1["battery-percentage"] = 100;
  drone1["distance"] = drone1Data->distance_traveled;
  drone1["deliveries"] = drone1Data->num_deliveries;

  obj["drone1"] = drone1;*/
  // obj["drone2"];
  // obj["drone3"];
  // obj["drone4"];

  return obj;
}

void DataCollection::Notify() { this->observer->Update(generateWebJSON()); }

void DataCollection::generateJSON() {
  std::cout << "Generating JSON file\n";
  /* Format of json file
      *Name of file is timestamp
      {
          "total_simulation_time": <total simulation time>,
          "Drone_1":{
              "distance_traveled": <Dist Traveled>,
              "number_of_charging_station_visits": <num charging station stops>,
              "number_of_deliveries": <num deliveries>,
              "delivery_times":[d1, d2, ... dn],
              "position_log":[
                  [x1, y1, z1],
                  ...
                  [xn, yn, zn]
              ]
          },
          ...
          "Drone_N":{...
          },
          "Robot_1":{
              "distance_traveled": <Dist Traveled>,
              "position_log":[
                  [x1, y1, z1],
                  ...
                  [xn, yn, zn]
              ]
          },
          ...
          "Robot_N":{...
          }
      }

  */
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
  outfile << "\"simulation_time\": " << total_elapsed_time << ",\n\t";
  outfile << "\"drones\": {";
  for (it = this->drone_data.begin(); it != this->drone_data.end(); it++) {
    outfile << "\n\t\t\"drone_" << i << "\": {\n\t\t\t";
    outfile << "\"distance_traveled\": " << it->second->distance_traveled
            << ",\n\t\t\t";
    outfile << "\"number_of_charging_station_visits\": "
            << it->second->num_charging_station_stops << ",\n\t\t\t";
    outfile << "\"number_of_deliveries\": " << it->second->num_deliveries
            << ",\n\t\t\t";
    outfile << "\"delivery_times\": [";
    size = it->second->delivery_times.size();
    for (int j = 0; j < size; j++) {
      outfile << it->second->delivery_times[i];
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
    if (std::next(it) == drone_data.end()) {
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
  for (it2 = this->robot_data.begin(); it2 != this->robot_data.end(); it2++) {
    outfile << "\"robot_" << i << "\": {\n\t\t\t";
    outfile << "\"distance_traveled\": " << it2->second->distance_traveled
            << ",\n\t\t\t";
    outfile << "\"positions\": [\n\t\t\t\t";
    size = it2->second->positions.size();
    for (int j = 0; j < size; j++) {
      outfile << "{" << it2->second->positions[j].x << ", "
              << it2->second->positions[j].y << ", "
              << it2->second->positions[j].z << "}";
      if (j != size - 1) {
        outfile << ",\n\t\t\t\t";
      }
    }
    outfile << "\n\t\t\t]";
    if (std::next(it2) == robot_data.end()) {
      outfile << "\n\t\t}";
    } else {
      outfile << "\n\t\t},";  // close "robot_i": {
    }
    i++;
  }

  outfile << "\n\t}";  // close "robots": {
  outfile << "\n}";    // close json data
  outfile.close();
  std::cout << "end" << std::endl;
}