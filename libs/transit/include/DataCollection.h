#ifndef BATTERY_DECORATOR_H_
#define BATTERY_DECORATOR_H_

#include <vector>
#include <ctime>
#include "math/vector3.h"
#include "IEntity.h"

//using namespace routing;

class DataCollection {
    public:
        /**
        * Singletons should not be assignable.
        */
        void operator=(const DataCollection &) = delete;
        /**
        * This is the static method that controls the access to the singleton
        * instance. On the first run, it creates a singleton object and places it
        * into the static field. On subsequent runs, it returns the client existing
        * object stored in the static field.
        */
        DataCollection* GetInstance();

        void addDrone(IEntity* drone);
        void addRobot(IEntity* robot);
        
        void generateCSV();

        // update number of charge station stops
        void addStationStop(IEntity *drone){

        }
        // update distance traveled for drone
        void updateDistanceDrone(IEntity* drone);
        // update number of passengers picked up
        void addPassenger(IEntity* drone);
        // add new position
        void addNewPositionDrone(IEntity* drone, Vector3 pos);
        // add a new time for delivery (uses current simulation time)
        void addNewDeliveryTime(IEntity* drone);

        // update distance traveled for robot
        void updateDistanceRobot(IEntity* robot);
        // add new position
        void addNewPositionRobot(IEntity* robot, Vector3 pos);

        void updateSimTime(double dt);


    private:
        
        DataCollection();
        static DataCollection* instancePtr;

        struct DroneData {
            DroneData(){
                num_deliveries = 0;
                distance_traveled = 0.0;
            }
            std::vector<Vector3> positions;
            std::vector<double> delivery_times;
            int num_deliveries;
            double distance_traveled;
        };

        struct RobotData {
            RobotData(){
                distance_traveled = 0.0;
            }
            double distance_traveled;
            std::vector<Vector3> positions;
        };

        double total_elapsed_time;
        std::map<std::string , DroneData> drone_data;
        std::map<std::string , RobotData> robot_data;

        
};

#endif