#pragma once

class ExponentialMovingAverage {
public:
    ExponentialMovingAverage(float alpha = 0.3f, float initialValue = 0.0f);
    void addValue(float newValue);
    float getAverage() const;
    void fill(float value);
    void setAlpha(float alpha);

private:
    float currentValue;
    float alpha;
};
