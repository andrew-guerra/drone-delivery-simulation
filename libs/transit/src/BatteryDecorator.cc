#include "../include/BatteryDecorator.h"

BatteryDecorator::BatteryDecorator(IEntity *entity){
    this->entity = entity;
    this->battery_life = MAX_BATTERY; 
    this->charging = false;
}

// BatteryDecorator::~BatteryDecorator(){
    // delete entity;
// }

void BatteryDecorator::Update(double dt, std::vector<IEntity*> scheduler){
    // Maybe battery drain is just a function of time. 
    // Then the drone finds the time it will take to travel the destination 

    if(charging){
        this->battery_life += 4*dt;
        if(battery_life >= MAX_BATTERY){
            charging = false;
        }
    }else{
        this->battery_life -= dt;
    }

    if(!entity->GetAvailability()){
        // If drone has picked up an entity then we can do calculations on that entity. 

    }


    entity->Update();
}

double BatteryDecorator::GetDistanceToDestination(IEntity *entity){
    std::string strategyName = entity->GetStrategyName();
    std::vector< std::vector<float> > path;
    std::vector<float> pos;
    std::vector<float> des;
    double distance = 0;
    
    pos[0] = entity->GetPosition().x;
    pos[1] = entity->GetPosition().y;
    pos[2] = entity->GetPosition().z;
    des[0] = entity->GetDestination().x;
    des[1] = entity->GetDestination().y;
    des[2] = entity->GetDestination().z;
    
    // get path based on routing strategy
    if (strategyName.compare("astar") == 0) {
        path = entity->GetGraph()->GetPath(pos, des, AStar::Default());

    } else if (strategyName.compare("dfs") == 0) {
        path = entity->GetGraph()->GetPath(pos, des, DepthFirstSearch::Default());

    } else if (strategyName.compare("dijkstra") == 0) {
        path = entity->GetGraph()->GetPath(pos, des, Dijkstra::Default());
    }

    // calculate the distance between every point
    for(int i = 0; i<path.size()-2; i++){
        distance += EuclideanDistance.Calculate(path[i], path[i+1]);
    }

    return distance;
}

IEntity * GetClosestChargingStation(std::vector<float> location, std::vector<IEntity*> scheduler){
    IEntity* closest;
    double minDist = 999999999999999999999;
    for(IEntity entity : scheduler){
        //might have to change id name
        if(entity.GetId().compare("batterycharger") == 0){
            double currDist = EuclideanDistance.Calculate(location, entity->GetPosition());
            if(currDist < minDist){
                minDist = currDist;
                closest = entity;
            }
        }   
    }
    return entity;
}

// Returns true if the drone can make it to the robot, to the robot's destination then to a battery station on it's current charge
bool BatteryDecorator::CanReachDestination(std::vector<IEntity*> scheduler){
    // Get the distance from the entity's position to its destination
    double distance = GetDistanceToDestination(entity->GetEntity());

    // Add in distance from the drone to the robot
    distance += EuclideanDistance.Calculate(entity->GetPosition(), entity->GetEntity()->GetPosition());

    // get the location of the closest charging station 
    std::vector<float> closestCharger = GetClosestChargingStation(entity->GetEntity()->GetDestination(), scheduler);

    // Add in the distance from the robot's location to the closest charger
    distance += EuclideanDistance.Calculate(closestCharger, entity->GetEntity->GetDestination());

    // drone speed = 30 units/time unit
    // Calculate the time it will take to reach the destination. 
    double time = distance / entity->GetSpeed();

    return time < battery_life;
}
