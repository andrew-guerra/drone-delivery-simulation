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
        void updateNumStationStops(IEntity *drone);
        // update distance traveled for drone
        void updateDistanceDrone(IEntity* drone);
        // update number of passengers picked up
        void updateNumPassengers(IEntity* drone);
        // add new position
        void addNewPositionDrone(IEntity* drone, Vector3 pos);
        // add a new time for delivery
        void addNewDeliveryTime(IEntity* drone, double dt);

        // update distance traveled for robot
        void updateDistanceRobot(IEntity* robot);
        // add new position
        void addNewPositionRobot(IEntity* robot, Vector3 pos);

        void updateSimTime(double dt);


    private:
        
        DataCollection();

        static DataCollection* instancePtr;

        struct DroneData {
            std::vector<Vector3> positions;
            std::vector<std::time_t> time_per_delivery;
            int num_deliveries;
            int distance_traveled;
        } droneData;

        struct RobotData {
            int distance_traveled;
            std::vector<Vector3> positions;
        } robotData;

        double total_elapsed_time;

        std::vector<DataCollection::DroneData> drone_data;
        
        std::vector<DataCollection::RobotData> robot_data;

        std::vector<IEntity*> drones;
        
        std::vector<IEntity*> robots;
        
};

#endif