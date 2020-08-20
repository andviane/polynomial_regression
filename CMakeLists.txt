cmake_minimum_required(VERSION 3.16)
project(polynomial_regression CXX)

set(CMAKE_CXX_STANDARD 17)
set(POLYNOMIAL_REGRESSION_SRC polynomial_regression.hpp)

add_library(polynomial_regression STATIC ${POLYNOMIAL_REGRESSION_SRC})
set_target_properties(polynomial_regression PROPERTIES LINKER_LANGUAGE CXX)

include(examples/CMakeLists.txt)

find_package(GTest)

if(GTEST_FOUND)
  # only build tests if GTest is available.
  include(tests/CMakeLists.txt)
endif()


