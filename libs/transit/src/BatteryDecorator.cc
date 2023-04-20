#include "../include/BatteryDecorator.h"

BatteryDecorator::BatteryDecorator(IEntity *entity){
    this->entity = entity;
}

// BatteryDecorator::~BatteryDecorator(){
    // delete entity;
// }

void BatteryDecorator::Update(double dt){
    this->battery_life -= dt;
    
}

double BatteryDecorator::GetStrategyDistance(){
    std::string strategyName = entity->GetStrategyName();
    EuclideanDistance dist(); //Declare a new EuclidianDistance obj called dist for the Calculate method
    std::vector< std::vector<float> > path;
    double distance = 0;
    
    if (strategyName.compare("astar") == 0) {
        path = entity->graph->GetPath(entity->GetPosition(),entity->GetDestination(), AStar::Default());

    } else if (strategyName.compare("dfs") == 0) {
        path = entity->graph->GetPath(entity->GetPosition(),entity->GetDestination(), DepthFirstSearch::Default());

    } else if (strategyName.compare("dijkstra") == 0) {
        path = entity->graph->GetPath(entity->GetPosition(),entity->GetDestination(), Dijkstra::Default());
    }

    //calculate the distance between every point
    for(int i = 0; i<path.size()-2; i++){
        distance += dist.Calculate(path[i], path[i+1]);
    }

    return distance;
}
