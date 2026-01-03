#pragma once

#include <stddef.h>
#include <vector>

class MovingAverage {
public:
    MovingAverage(int sampleSize);
    void addValue(float newValue);
    float getAverage() const;
    void fill(float value);
    void resize(size_t newSize);

private:
    std::vector<float> values;
    size_t head = 0;
};
