#ifndef HELICOPTER_H
#define HELICOPTER_H

#include <vector>

#include "BeelineStrategy.h"
#include "IEntity.h"
#include "IStrategy.h"
#include "math/vector3.h"
#include "util/json.h"

/**
 * @brief Helicopter that goes float float chop chop
 * Spawn one in when sim is started
 */
class Helicopter : public IEntity {
 public:
  /**
   * @brief Construct a new Helicopter object
   *
   * @param obj
   */
  Helicopter(JsonObject& obj);
  /**
   * @brief Destroy the Helicopter object
   *
   */
  ~Helicopter();
  /**
   * @brief Get the Speed float
   *
   * @return float
   */
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
   * @brief Get a random destination for the helicopter to go to
   *
   * @return Vector3
   */
  Vector3 GetRandomDest();
  /**
   * @brief Get the Details of the object
   *
   * @return JsonObject
   */
  JsonObject GetDetails() const { return details; }
  /**
   * @brief Move the helicopter with the sim
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
   * @brief Rotate the helicopter based on some angle
   *
   * @param angle
   */
  void Rotate(double angle);
  /**
   * @brief Delete copy constructor
   *
   * @param helicopter
   */
  Helicopter(const Helicopter& helicopter) = delete;
  /**
   * @brief Delete equals op
   *
   * @param helicopter
   * @return Helicopter&
   */
  Helicopter& operator=(const Helicopter& helicopter) = delete;

 private:
  JsonObject details;
  Vector3 position;
  Vector3 direction;
  Vector3 destination;
  bool available = true;
  float speed;
  IStrategy* pathStrategy;
};

#endif
