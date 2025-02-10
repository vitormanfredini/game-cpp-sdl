include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(MovingAverage_test test_MovingAverage.cpp)

target_link_libraries(MovingAverage_test PRIVATE MovingAverage_lib gtest_main)

include(GoogleTest)
gtest_discover_tests(MovingAverage_test)