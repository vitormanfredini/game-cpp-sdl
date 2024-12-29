#pragma once

#include <vector>
#include <stddef.h>

class MovingAverage {
public:
    MovingAverage(int sampleSize) {
        values.resize(sampleSize);
        for(size_t c = 0; c < values.size(); c++){
            values[c] = 0;
        }
    }

    void addValue(float newValue){
        if(values.size() == 0){
            return;
        }
        values[head] = newValue;
        head++;
        if(head >= values.size()){
            head = 0;
        }
    }

    float getAverage(){
        float sum = 0;
        for(size_t c = 0; c < values.size(); c++){
            sum += values[c];
        }
        return sum / static_cast<float>(values.size());
    }

private:
    std::vector<float> values;
    size_t head = 0;
};
