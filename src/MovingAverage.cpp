#include "MovingAverage.h"
#include <stddef.h>
#include <stdexcept>
#include <iostream>


MovingAverage::MovingAverage(int sampleSize) {
    if (sampleSize <= 0) {
        throw std::invalid_argument("Sample size must be greater than zero.");
    }
    values.resize(sampleSize);
    for(size_t c = 0; c < values.size(); c++){
        values[c] = 0;
    }
}

void MovingAverage::addValue(float newValue){
    if(values.size() == 0){
        return;
    }
    values[head] = newValue;
    head++;
    if(head >= values.size()){
        head = 0;
    }
}

float MovingAverage::getAverage(){
    float sum = 0;
    for(size_t c = 0; c < values.size(); c++){
        sum += values[c];
    }
    return sum / static_cast<float>(values.size());
}

void MovingAverage::fill(float value){
    for(size_t c = 0; c < values.size(); c++){
        values[c] = value;
    }
}

void MovingAverage::resize(size_t newSize){
    values.resize(newSize);
    if(head >= newSize){
        head = 0;
    }
}
