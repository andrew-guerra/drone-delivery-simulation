#ifndef HORSE_H_
#define HORSE_H_

#include <vector>

#include "DijkstraStrategy.h"
#include "IEntity.h"
#include "IStrategy.h"
#include "math/vector3.h"
#include "util/json.h"

/**
 * @brief A spawnable creature that walks around like a lil guy
 *
 */
class Horse : public IEntity {
 public:
  /**
   * @brief Construct a new Horse object
   *
   * @param obj json details
   */
  Horse(JsonObject& obj);
  /**
   * @brief Destroy the Horse object
   *
   */
  ~Horse();
  /**
   * @brief Gets the speed of the Horse
   * @return The speed of the Horse
   */
  float GetSpeed() const { return speed; }

  /**
   * @brief Gets the position of the Horse
   * @return The position of the Horse
   */
  Vector3 GetPosition() const { return position; }

  /**
   * @brief Gets the direction of the Horse
   * @return The direction of the Horse
   */
  Vector3 GetDirection() const { return direction; }

  /**
   * @brief Gets the destination of the Horse
   * @return The destination of the Horse
   */
  Vector3 GetDestination() const { return destination; }

  /**
   * @brief Gets the details information of the Horse
   * @return The details information of the Horse
   */
  JsonObject GetDetails() const { return details; }

  /**
   * @brief Gets the availability of the Horse
   * @return The availability of the Horse
   */
  bool GetAvailability() const { return available; }

  /**
   * @brief Get a random destination on the map
   *
   * @return Vector3
   */
  Vector3 GetRandomDest();

  /**
   * @brief Updates the Horse's position
   * @param dt Delta time
   * @param scheduler Vector containing all the entities in the system
   */
  void Update(double dt, std::vector<IEntity*> scheduler);

  /**
   * @brief Sets the position of the Horse
   * @param pos_ The new position of the Horse
   */
  void SetPosition(Vector3 pos_) { position = pos_; }

  /**
   * @brief Sets the direction of the Horse
   * @param dir_ The new direction of the Horse
   */
  void SetDirection(Vector3 dir_) { direction = dir_; }

  /**
   * @brief Sets the destination of the Horse
   * @param des_ The new destination of the Horse
   */
  void SetDestination(Vector3 des_) { destination = des_; }

  /**
   * @brief Rotates the Horse by a specified angle
   *
   * @param angle Angle to rotate Horse by
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
