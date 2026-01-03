#include "test_config.h"
#include "../src/ExponentialMovingAverage.h"
#include <gtest/gtest.h>

TEST(ExponentialMovingAverageTest, InitialValue) {
    ExponentialMovingAverage ema(0.5f, 10.0f);
    EXPECT_NEAR(ema.getAverage(), 10.0f, TEST_EPSILON);
}

TEST(ExponentialMovingAverageTest, Fill) {
    ExponentialMovingAverage ema(0.3f, 0.0f);
    ema.fill(5.0f);
    EXPECT_NEAR(ema.getAverage(), 5.0f, TEST_EPSILON);
}

TEST(ExponentialMovingAverageTest, AddValueCalculation) {
    ExponentialMovingAverage ema(0.5f, 10.0f);
    
    // First update: 0.5 * 20 + 0.5 * 10 = 10 + 5 = 15
    ema.addValue(20.0f);
    EXPECT_NEAR(ema.getAverage(), 15.0f, TEST_EPSILON);
    
    // Second update: 0.5 * 10 + 0.5 * 15 = 5 + 7.5 = 12.5
    ema.addValue(10.0f);
    EXPECT_NEAR(ema.getAverage(), 12.5f, TEST_EPSILON);
}

TEST(ExponentialMovingAverageTest, SetAlpha) {
    ExponentialMovingAverage ema(0.5f, 10.0f);
    ema.setAlpha(0.8f);
    
    // 0.8 * 20 + 0.2 * 10 = 16 + 2 = 18
    ema.addValue(20.0f);
    EXPECT_NEAR(ema.getAverage(), 18.0f, TEST_EPSILON);
}

TEST(ExponentialMovingAverageTest, InvalidAlpha) {
    EXPECT_THROW(ExponentialMovingAverage(0.0f, 10.0f), std::invalid_argument);
    EXPECT_THROW(ExponentialMovingAverage(-0.1f, 10.0f), std::invalid_argument);
    EXPECT_THROW(ExponentialMovingAverage(1.5f, 10.0f), std::invalid_argument);
    
    ExponentialMovingAverage ema(0.5f, 10.0f);
    EXPECT_THROW(ema.setAlpha(0.0f), std::invalid_argument);
    EXPECT_THROW(ema.setAlpha(-0.1f), std::invalid_argument);
    EXPECT_THROW(ema.setAlpha(1.5f), std::invalid_argument);
}

TEST(ExponentialMovingAverageTest, SmoothingBehavior) {
    // Test with low alpha (more smoothing)
    ExponentialMovingAverage emaSmooth(0.1f, 0.0f);
    emaSmooth.addValue(100.0f);
    EXPECT_NEAR(emaSmooth.getAverage(), 10.0f, TEST_EPSILON);
    
    // Test with high alpha (less smoothing, more responsive)
    ExponentialMovingAverage emaResponsive(0.9f, 0.0f);
    emaResponsive.addValue(100.0f);
    EXPECT_NEAR(emaResponsive.getAverage(), 90.0f, TEST_EPSILON);
}
