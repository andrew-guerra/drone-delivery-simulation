#include "../include/BatteryDecorator.h"

BatteryDecorator::BatteryDecorator(IEntity *entity){
    this->entity = entity;
    this->battery_life = MAX_BATTERY; 
    this->charging = false;
    this->entity_dest = Vector3();
    this->needToCalcDist = true;
    this->canReach = true;
}

void BatteryDecorator::AddSimEntities(std::vector<IEntity*> entities){
    this->sim_entities = entities;
}

double BatteryDecorator::GetDistanceToDestination(IEntity *entity){
    // std::cout << "getdistToDestination" << std::endl;
    std::string strategyName = entity->GetStrategyName();
    std::vector< std::vector<float> > path;
    std::vector<float> pos;
    std::vector<float> des;
    EuclideanDistance eucDist;
    double distance = 0;

    pos.push_back(entity->GetPosition().x);
    pos.push_back(entity->GetPosition().y);
    pos.push_back(entity->GetPosition().z);
    des.push_back(entity->GetDestination().x);
    des.push_back(entity->GetDestination().y);
    des.push_back(entity->GetDestination().z);

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
        distance += eucDist.Calculate(path[i], path[i+1]);
    }

    return distance;
}

IEntity * BatteryDecorator::GetClosestChargingStation(std::vector<float> location, std::vector<IEntity*> entities){
    IEntity* closest = NULL;
    EuclideanDistance eucDist;
    double minDist = 9999999999999999.0;
    for(int i = 0; i<sim_entities.size(); i++){
        //Get the entity's details 
        JsonObject details = sim_entities[i]->GetDetails();
        std::string name = details["type"];
        //If the entity is a charging station then get its position and compare
        if(name.compare("charging_station") == 0){
            // std::cout << "found one" << std::endl;
            std::vector<float> curr;
            curr.push_back(sim_entities[i]->GetPosition().x);
            curr.push_back(sim_entities[i]->GetPosition().y);
            curr.push_back(sim_entities[i]->GetPosition().z);
            double currDist = eucDist.Calculate(location, curr);
            if(currDist < minDist){
                minDist = currDist;
                closest = sim_entities[i];
            }
        }   
    }
    return closest;
}

// Returns true if the drone can make it to the robot, to the robot's destination then to a battery station on it's current charge
bool BatteryDecorator::CanReachDestination(){
    // Get the distance from the entity's position to its destination
    double distance = GetDistanceToDestination(entity->GetEntity());

    // Add in distance from the drone to the robot
    distance += entity->GetPosition().Distance(entity->GetEntity()->GetPosition());

    std::vector<float> curPos;
    curPos.push_back(entity->GetEntity()->GetDestination().x);
    curPos.push_back(entity->GetEntity()->GetDestination().y);
    curPos.push_back(entity->GetEntity()->GetDestination().z);

    // get the location of the closest charging station 
    IEntity* closestCharger = GetClosestChargingStation(curPos, sim_entities);
    if(closestCharger == NULL){
        std::cout << "no charger found" << std::endl;
    }

    // Add in the distance from the robot's location to the closest charger
    Vector3 dest = entity->GetEntity()->GetDestination();
    Vector3 pos = closestCharger->GetPosition();
    dest.Print();
    distance += pos.Distance(dest);


    // drone speed = 30 units/time unit
    // Calculate the time it will take to reach the destination. 
    double time = distance / entity->GetSpeed();

    return time < battery_life;
}

void BatteryDecorator::Update(double dt, std::vector<IEntity*> scheduler){
    // Maybe battery drain is just a function of time. 
    // Then the drone finds the time it will take to travel the destination 
    std::cout << battery_life << std::endl;

    options opts;

    if(charging){
        opts = Charging;

    }else if(!entity->GetAvailability()){
        if(needToCalcDist){
            canReach = CanReachDestination();
            needToCalcDist = false;
        }
        if(canReach){
            opts = Default;
        }else{
            opts = toCharger;
        }

    }else{
        //Drone unavail and not charging
        opts = Default;
        needToCalcDist = true;
    }

    switch(opts){
        case Charging:
            std::cout << "charging" << std::endl;
            this->battery_life += 4*dt;
            if(battery_life >= MAX_BATTERY){
                charging = false;
                //set the drone's destination back to the robot
                entity->SetDestination(entity_dest);
                //reset entity_dest's y val to 0
                entity_dest.y = 0;
            }
            break;
        case toCharger:
            std::cout << "to charger" << std::endl;
            //decrease battery
            battery_life -= dt;
            
            //if the entity_dest's y is 0 it's not a valid destination (unless we make tunnels in the future);
            if(entity_dest.y == 0){
                //store the entity's old destination 
                entity_dest = entity->GetDestination();
                std::vector<float> pos;
                pos.push_back(entity->GetPosition().x);
                pos.push_back(entity->GetPosition().y);
                pos.push_back(entity->GetPosition().z);
                //find the closest charging station
                IEntity *charger = GetClosestChargingStation(pos, sim_entities);
                //set the new destination to be the charger
                entity->SetDestination(charger->GetPosition());

            }else if(entity->GetPosition().Distance(entity->GetDestination()) < 1){
                //here we have reached the charging station
                charging = true;

            }else{
                IStrategy *pathStrategy = new BeelineStrategy(entity->GetPosition(), entity->GetDestination());
                pathStrategy->Move(entity, dt);
            }
            break;
        case Default:
            std::cout << "dflt" << std::endl;
            //decrease battery
            battery_life -= dt;
            entity->Update(dt, scheduler);
            break;
    }

    if(battery_life < 0){
        std::cout << "uh oh" << std::endl;
    }
}

Vector3 BatteryDecorator::GetPosition() const {
    return entity->GetPosition();
}

Vector3 BatteryDecorator::GetDirection() const {
    return entity->GetDirection();
}

Vector3 BatteryDecorator::GetDestination() const {
    return entity->GetDestination();
}

JsonObject BatteryDecorator::GetDetails() const {
    return entity->GetDetails();
}

float BatteryDecorator::GetSpeed() const {
    return entity->GetSpeed();
}