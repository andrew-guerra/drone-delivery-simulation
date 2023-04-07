#include "../include/SpinDecorator.h"

SpinDecorator::SpinDecorator(IStrategy* strategy) : CelebrationDecorator(strategy) {
    time = 0;
}


void SpinDecorator::Move(IEntity* entity, double dt) {
    // Spin the entity
    if(this->strategy->IsCompleted()){
        while (time < 4){
            entity->Rotate( .5* dt);
            time += dt;
            //std::cout << time << std::endl;
        }
    }else{
        this->strategy->Move(entity, dt);
    }
}

bool SpinDecorator::IsCompleted(){
    return time >= 4;
}
