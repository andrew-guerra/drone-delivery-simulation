#include "DataCollection.h"
DataCollection::~DataCollection(){
    std::map<std::string, DroneData*>::iterator it;
    std::map<std::string, RobotData*>::iterator it2;


    for (it = this->drone_data.begin(); it != this->drone_data.end(); it++){
        delete it->second;
    }

    for (it2 = this->robot_data.begin(); it2 != this->robot_data.end(); it2++){
        delete it2->second;
    }
}

DataCollection* DataCollection::GetInstance() {
    if (instancePtr == NULL) {
        instancePtr = new DataCollection();
    } 
    return instancePtr;
}

DataCollection::DataCollection() {
    this->total_elapsed_time = 0.0;
}

void DataCollection::addDrone(IEntity* drone){
    DroneData *new_drone_data = new DroneData();
    JsonObject details = drone->GetDetails();
    std::string name = details["name"];

    // Map the drone's name to the new DroneData object
    this->drone_data[name] = new_drone_data;
}

void DataCollection::addRobot(IEntity* robot){
    RobotData *new_robot_data = new RobotData();
    JsonObject details = robot->GetDetails();
    std::string name = details["name"];

    // Map the robot's name to the new RobotData object
    this->robot_data[name] = new_robot_data;
}

void DataCollection::addStationStop(IEntity *drone){
    JsonObject details = drone->GetDetails();
    std::string name = details["name"];

    this->drone_data[name]->num_charging_station_stops++;
}

void DataCollection::addDelivery(IEntity* drone){
    JsonObject details = drone->GetDetails();
    std::string name = details["name"];

    this->drone_data[name]->num_deliveries++;
}

void DataCollection::addNewDeliveryTime(IEntity* drone){
    // add the new delivery time in terms of global simulation time
    JsonObject details = drone->GetDetails();
    std::string name = details["name"];

    this->drone_data[name]->delivery_times.push_back(this->total_elapsed_time);
}

void DataCollection::addNewPositionDrone(IEntity* drone, Vector3 pos){
    JsonObject details = drone->GetDetails();
    std::string name = details["name"];
    std::vector<Vector3> *positions = &this->drone_data[name]->positions;

    Vector3 previous_position = positions->back(); // The last element of the vector

    this->drone_data[name]->positions.push_back(pos); // Add the new position to the vector

    this->drone_data[name]->distance_traveled += previous_position.Distance(pos); // Add the (linear) distance between the two positions
}

void DataCollection::addNewPositionRobot(IEntity* robot, Vector3 pos){
    JsonObject details = robot->GetDetails();
    std::string name = details["name"];
    std::vector<Vector3> *positions = &this->robot_data[name]->positions;

    Vector3 previous_position = positions->back(); // The last element of the vector

    this->robot_data[name]->positions.push_back(pos); // Add the new position to the vector

    this->robot_data[name]->distance_traveled += previous_position.Distance(pos); // Add the (linear) distance between the two positions}
}

// updates the total time of the simulation
void DataCollection::updateSimTime(double dt){
    this->total_elapsed_time += dt;
}

void DataCollection::generateJSON(){
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

    // Creates a new timestamp of the current date and time to use as the file name
    std::time_t now = std::time(nullptr);
    char timestamp[20];
    std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d::%H-%M-%S", std::localtime(&now));
    std::string stamp = std::string(timestamp);
    stamp += ".json";

    ofstream outfile(stamp);
    if(!outfile){
        std::cout<< "Failed to open json file for output" << std::endl;
        exit(1);
    }

    outfile << "{\n\t";
    outfile << "\"simulation_time\": " << total_elapsed_time << ",\n\t";
    outfile << "\"drones\": {\n\t\t";
    for (it = this->drone_data.begin(); it != this->drone_data.end(); it++){
        outfile << "\"drone_" << i << "\": {\n\t\t\t";
        outfile << "\"distance_traveled\": " << it->second->distance_traveled << ",\n\t\t\t";
        outfile << "\"number_of_charging_station_visits\": " << it->second->num_charging_station_stops << ",\n\t\t\t";
        outfile << "\"number_of_deliveries\": " << it->second->num_deliveries << ",\n\t\t\t";
        outfile << "\"delivery_times\": [";
        size = it->second->delivery_times.size();
        for(int j = 0; j<size; j++){
            outfile << it->second->delivery_times[i];
            if(j != size-1){
                outfile << ", ";
            }
        }
        outfile << "],\n\t\t\t";
        outfile << "\"positions\": [\n\t\t\t\t";
        size = it->second->positions.size();
        for(int j = 0; j<size; j++){
            outfile << it->second->positions[i].Print();
            if(j != size - 1){
                outfile << ",\n\t\t\t\t";
            }
        }
        outfile << "\n\t\t\t]";
        outfile << "\n\t\t}," // close "drone_i": {
        i++;
    }
    outfile << "\n\t},"; // close "drones": {
    // Begin outputting robot data
    outfile << "\"robots\": {\n\t\t";
    i = 1;
    for (it2 = this->robot_data.begin(); it2 != this->robot_data.end(); it2++){
        outfile << "\"robot_" << i << "\": {\n\t\t\t";
        outfile << "\"distance_traveled\": " << it2->second->distance_traveled << ",\n\t\t\t";
        outfile << "\"positions\": [\n\t\t\t\t";
        size = it2->second->positions.size();
        for(int j = 0; j<size; j++){
            outfile << it2->second->positions[i].Print();
            if(j != size - 1){
                outfile << ",\n\t\t\t\t";
            }
        }
        outfile << "\n\t\t\t]";
        outfile << "\n\t\t}," // close "robot_i": {
        i++;
    }
    outfile << "\n\t},"; // close "robots": {
    outfile << "\n}"; // close json data
    outfile.close();
}