cmake_minimum_required(VERSION 3.16)
project(polynomial_regression_tests)

set(CMAKE_CXX_STANDARD 17)
find_package(GTest)

set(TEST_SRC
  tests/test_fit.cpp
  tests/test_interpolate.cpp
  tests/test_diff_integr.cpp
)

add_executable(tests ${TEST_SRC} ${POLYNOMIAL_REGRESSION_SRC})
target_link_libraries(tests gtest gtest_main pthread)
target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
