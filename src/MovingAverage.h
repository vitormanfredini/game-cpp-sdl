#pragma once

#include <stddef.h>
#include <vector>

class MovingAverage {
public:
    MovingAverage(int sampleSize);
    void addValue(float newValue);
    float getAverage();
    void fill(float value);

private:
    std::vector<float> values;
    size_t head = 0;
};
