cmake_minimum_required(VERSION 3.16)
project(polynomial_regression_examples)

set(CMAKE_CXX_STANDARD 20)

add_executable(polynomial_examples examples/simple_main.cpp ${POLYNOMIAL_REGRESSION_SRC})
