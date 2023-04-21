#include "../include/BatteryDecorator.h"

BatteryDecorator::BatteryDecorator(IEntity *entity){
    this->entity = entity;
}

// BatteryDecorator::~BatteryDecorator(){
    // delete entity;
// }

void BatteryDecorator::Update(double dt){
    //Maybe battery drain is just a function of time. 
    //Then the drone finds the time it will take to travel the destination 
    this->battery_life -= dt;
}

double BatteryDecorator::GetDistanceToDestination(){
    std::string strategyName = entity->GetStrategyName();
    EuclideanDistance dist = EuclideanDistance(); //Declare a new EuclidianDistance obj called dist for the Calculate method
    std::vector< std::vector<float> > path;
    double distance = 0;
    std::vector<float> pos;
    std::vector<float> des;
    
    pos[0] = entity->GetPosition().x;
    pos[1] = entity->GetPosition().y;
    pos[2] = entity->GetPosition().z;
    des[0] = entity->GetDestination().x;
    des[1] = entity->GetDestination().y;
    des[2] = entity->GetDestination().z;
    
    if (strategyName.compare("astar") == 0) {
        path = entity->GetGraph()->GetPath(pos, des, AStar::Default());

    } else if (strategyName.compare("dfs") == 0) {
        path = entity->GetGraph()->GetPath(pos, des, DepthFirstSearch::Default());

    } else if (strategyName.compare("dijkstra") == 0) {
        path = entity->GetGraph()->GetPath(pos, des, Dijkstra::Default());
    }

    //calculate the distance between every point
    for(int i = 0; i<path.size()-2; i++){
        distance += dist.Calculate(path[i], path[i+1]);
    }

    return distance;
}

bool BatteryDecorator::CanReachDestination(){
    double distance = GetDistanceToDestination();
    
}
