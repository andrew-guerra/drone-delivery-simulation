#ifndef HORSE_FACTORY_H_
#define HORSE_FACTORY_H_

#include <vector>

#include "Horse.h"
#include "IEntityFactory.h"

/**
 * @brief Horse factory to create Horses
 *
 */
class HorseFactory : public IEntityFactory {
 public:
  /**
   * @brief Destroy the Horse Factory object
   *
   */
  virtual ~HorseFactory() {}
  /**
   * @brief Creates entity using the given JSON object, if possible.
   * @param entity - JsonObject to be used to create the new entity.
   * @return Entity that was created if it was created successfully, or a
   *nullpointer if creation failed.
   **/
  IEntity* CreateEntity(JsonObject& entity);
};

#endif
