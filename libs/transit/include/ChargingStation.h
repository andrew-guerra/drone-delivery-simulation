#ifndef CHARGING_STATION_H_
#define CHARGING_STATION_H_

#include "IEntity.h"
#include "math/vector3.h"

/**
 * @brief The ChargingStation object itself
 * 
 * Holds the required IEntity virtual functionality, but 
 * the ChargingStation does not actually do much work
 * 
 */
class ChargingStation : public IEntity {

    public:
        /**
         * @brief Construct a new Charging Station object
         * 
         * @param obj 
         */
        ChargingStation(JsonObject& obj);
        /**
         * @brief Destroy the Charging Station object
         * 
         */
        ~ChargingStation();
        float GetSpeed() const { return speed; }
        /**
         * @brief Get the Position vector
         *
         * @return Vector3
         */
        Vector3 GetPosition() const { return position; }
        /**
         * @brief Get the Direction vector
         *
         * @return Vector3
         */
        Vector3 GetDirection() const { return direction; }
        /**
         * @brief Get the Destination vector
         *
         * @return Vector3
         */
        Vector3 GetDestination() const { return destination; }
        /**
         * @brief Get the Details of the object
         *
         * @return JsonObject
         */
        JsonObject GetDetails() const { return details; }
        /**
         * @brief Move the ChargingStation with the sim
         *
         * @param dt sim time
         * @param scheduler entity scheduler/interaction
         */
        void Update(double dt, std::vector<IEntity*> scheduler);
        /**
         * @brief Set the Position vector
         *
         * @param pos_
         */
        void SetPosition(Vector3 pos_) { position = pos_; }
        /**
         * @brief Set the Direction vector
         *
         * @param dir_
         */
        void SetDirection(Vector3 dir_) { direction = dir_; }
        /**
         * @brief Set the Destination vector
         *
         * @param des_
         */
        void SetDestination(Vector3 des_) { destination = des_; }
        /**
         * @brief Delete copy constructor
         *
         * @param ChargingStation
         */
        ChargingStation(const ChargingStation& chargingStation) = delete;
        /**
         * @brief Delete equals op
         *
         * @param ChargingStation
         * @return ChargingStation&
         */
        ChargingStation& operator=(const ChargingStation& chargingStation) = delete;

    private:
        JsonObject details;
        Vector3 position;
        Vector3 direction;
        Vector3 destination;
        float speed = 0;
};

#endif