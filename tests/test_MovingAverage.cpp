#include "test_config.h"
#include "../src/MovingAverage.h"
#include <gtest/gtest.h>

TEST(MovingAverageTest, FillSamples) {
    MovingAverage ma(3);
    ma.fill(0.611);
    EXPECT_NEAR(ma.getAverage(), 0.611, TEST_EPSILON);
}

TEST(MovingAverageTest, AverageCalculation) {
    MovingAverage ma3(3);
    ma3.addValue(0.3);
    ma3.addValue(0.4);
    ma3.addValue(0.5);
    EXPECT_NEAR(ma3.getAverage(), 0.4, TEST_EPSILON);

    MovingAverage ma5(5);
    ma5.addValue(0.1);
    ma5.addValue(0.1);
    ma5.addValue(0.5);
    ma5.addValue(0.5);
    ma5.addValue(0.3);
    EXPECT_NEAR(ma5.getAverage(), 0.3, TEST_EPSILON);
}
