#ifndef HUMAN_H_
#define HUMAN_H_

#include <vector>

#include "AstarStrategy.h"
#include "IEntity.h"
#include "IStrategy.h"
#include "math/vector3.h"
#include "util/json.h"

/**
 * @brief A spawnable creature that walks around like a lil guy
 *
 */
class Human : public IEntity {
 public:
  /**
   * @brief Construct a new Human object
   *
   * @param obj json details
   */
  Human(JsonObject& obj);
  /**
   * @brief Destroy the Human object
   *
   */
  ~Human();
  /**
   * @brief Gets the speed of the human
   * @return The speed of the human
   */
  float GetSpeed() const { return speed; }

  /**
   * @brief Gets the position of the human
   * @return The position of the human
   */
  Vector3 GetPosition() const { return position; }

  /**
   * @brief Gets the direction of the human
   * @return The direction of the human
   */
  Vector3 GetDirection() const { return direction; }

  /**
   * @brief Gets the destination of the human
   * @return The destination of the human
   */
  Vector3 GetDestination() const { return destination; }

  /**
   * @brief Gets the details information of the human
   * @return The details information of the human
   */
  JsonObject GetDetails() const { return details; }

  /**
   * @brief Gets the availability of the human
   * @return The availability of the human
   */
  bool GetAvailability() const { return available; }

  /**
   * @brief Get a random destination on the map
   *
   * @return Vector3
   */
  Vector3 GetRandomDest();

  /**
   * @brief Updates the human's position
   * @param dt Delta time
   * @param scheduler Vector containing all the entities in the system
   */
  void Update(double dt, std::vector<IEntity*> scheduler);

  /**
   * @brief Sets the position of the human
   * @param pos_ The new position of the human
   */
  void SetPosition(Vector3 pos_) { position = pos_; }

  /**
   * @brief Sets the direction of the human
   * @param dir_ The new direction of the human
   */
  void SetDirection(Vector3 dir_) { direction = dir_; }

  /**
   * @brief Sets the destination of the human
   * @param des_ The new destination of the human
   */
  void SetDestination(Vector3 des_) { destination = des_; }

  /**
   * @brief Rotates the human by a specified angle
   *
   * @param angle Angle to rotate human by
   */
  void Rotate(double angle);

 private:
  JsonObject details;
  Vector3 position;
  Vector3 direction;
  Vector3 destination;
  bool available;
  float speed;
  IStrategy* pathStrategy;
};

#endif
