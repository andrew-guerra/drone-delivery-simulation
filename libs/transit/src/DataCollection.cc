#include "DataCollection.h"

DataCollection* DataCollection::GetInstance() {
    if (instancePtr == NULL) {
        instancePtr = new DataCollection();
    } 
    return instancePtr;
}
DataCollection::DataCollection() {
    total_elapsed_time = 0.0;
    
}
