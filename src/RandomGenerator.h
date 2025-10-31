#pragma once

#include <random>
#include <mutex>

class RandomGenerator {
public:
    static RandomGenerator& getInstance() {
        static RandomGenerator instance;
        return instance;
    }

    float getRandom() {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_dist(m_engine);
    }

    int getRandomInt(int min, int max) {
        std::lock_guard<std::mutex> lock(m_mutex);
        std::uniform_int_distribution<int> dist(min, max);
        return dist(m_engine);
    }

private:

    RandomGenerator()
        : m_engine(std::random_device{}()),
          m_dist(0.0f, 1.0f)
    {}

    RandomGenerator(const RandomGenerator&) = delete;
    RandomGenerator& operator=(const RandomGenerator&) = delete;

    std::mt19937 m_engine;
    std::uniform_real_distribution<float> m_dist;
    std::mutex m_mutex;
};