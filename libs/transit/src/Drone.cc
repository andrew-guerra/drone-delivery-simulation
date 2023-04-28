#define _USE_MATH_DEFINES
#include "Drone.h"

#include <cmath>
#include <limits>

Drone::Drone(JsonObject& obj) : details(obj) {
  JsonArray pos(obj["position"]);
  position = {pos[0], pos[1], pos[2]};
  JsonArray dir(obj["direction"]);
  direction = {dir[0], dir[1], dir[2]};

  speed = obj["speed"];

  available = true;
}

Drone::~Drone() {
  // Delete dynamically allocated variables
  delete graph;
  delete nearestEntity;
  delete pathStrategy;
}

void Drone::GetNearestEntity(std::vector<IEntity*> scheduler) {
  float minDis = std::numeric_limits<float>::max();
  for (auto entity : scheduler) {
    if (entity->GetAvailability()) {
      float disToEntity = this->position.Distance(entity->GetPosition());
      if (disToEntity <= minDis) {
        minDis = disToEntity;
        nearestEntity = entity;
      }
    }
  }

  if (nearestEntity) {
    nearestEntity->SetAvailability(
        false);  // set availability to the nearest entity
    available = false;
    pickedUp = false;
    destination = nearestEntity->GetPosition();
  }
}

void Drone::Update(double dt, std::vector<IEntity*> scheduler) {
  // std::cout << "drone update" << std::endl;
  if (available) {
    GetNearestEntity(scheduler);
  }
  // if nearest entity found
  if (nearestEntity) {
    // if at destination
    if (position.Distance(destination) < 1) {
      if (!pickedUp) {
        pickedUp = true;
        available = false;
        pathStrategy = NULL;
        destination = nearestEntity->GetDestination();
        std::string strategyName = nearestEntity->GetStrategyName();
        if (strategyName.compare("astar") == 0) {
          pathStrategy = new AstarStrategy(position, destination, graph);
          pathStrategy = new JumpDecorator(pathStrategy);
        } else if (strategyName.compare("dfs") == 0) {
          pathStrategy = new DfsStrategy(position, destination, graph);
          pathStrategy = new JumpDecorator(pathStrategy);
          pathStrategy = new SpinDecorator(pathStrategy);
        } else if (strategyName.compare("dijkstra") == 0) {
          pathStrategy = new DijkstraStrategy(position, destination, graph);
          pathStrategy = new SpinDecorator(pathStrategy);
          pathStrategy = new JumpDecorator(pathStrategy);
        }
      } else {
        pickedUp = false;
        nearestEntity->SetAvailability(false);
        nearestEntity = NULL;
        pathStrategy = NULL;
        available = true;
        return;
      }
    }
    if (pickedUp) {
      pathStrategy->Move(this, dt);
      nearestEntity->SetPosition(position);
      if(pathStrategy->IsCompleted()){  //when strategy completes, update availability
        available = true;
      }
    } else {
      // use bee line strategy to move to robot
      pathStrategy = new BeelineStrategy(position, destination);
      pathStrategy->Move(this, dt);
    }
  }
}

void Drone::Rotate(double angle) {
  Vector3 dirTmp = direction;
  direction.x = dirTmp.x * std::cos(angle) - dirTmp.z * std::sin(angle);
  direction.z = dirTmp.x * std::sin(angle) + dirTmp.z * std::cos(angle);
}

void Drone::Jump(double height) {
  if (goUp) {
    position.y += height;
    jumpHeight += height;
    if (jumpHeight > 5) {
      goUp = false;
    }
  } else {
    position.y -= height;
    jumpHeight -= height;
    if (jumpHeight < 0) {
      goUp = true;
    }
  }
}
