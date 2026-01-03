#include "ExponentialMovingAverage.h"
#include <stdexcept>

ExponentialMovingAverage::ExponentialMovingAverage(float alpha, float initialValue) 
    : currentValue(initialValue), alpha(alpha) {
    if (alpha <= 0.0f || alpha > 1.0f) {
        throw std::invalid_argument("Alpha must be between 0 (exclusive) and 1 (inclusive).");
    }
}

void ExponentialMovingAverage::addValue(float newValue) {
    currentValue = alpha * newValue + (1.0f - alpha) * currentValue;
}

float ExponentialMovingAverage::getAverage() const {
    return currentValue;
}

void ExponentialMovingAverage::fill(float value) {
    currentValue = value;
}

void ExponentialMovingAverage::setAlpha(float alpha) {
    if (alpha <= 0.0f || alpha > 1.0f) {
        throw std::invalid_argument("Alpha must be between 0 (exclusive) and 1 (inclusive).");
    }
    this->alpha = alpha;
}
