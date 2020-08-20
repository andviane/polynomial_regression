#include <deque>
#include "gtest/gtest.h"

#include "polynomial_regression.hpp"

TEST(Fit, n0) {
  // y = f(x) = c
  float c = 4;

  std::vector<float> x;
  std::vector<float> y;

  for (int xx = -10; xx < 10; xx++) {
    x.push_back(xx);
    y.push_back(c);
  }

  Polynomial<0, float> p = polynomial_regression<0, float>(x, y);

  ASSERT_FLOAT_EQ(p[0], c);

}

// Linear regression
TEST(Fit, n1) {
  // y = f(x) = ax + c
  float a = 2;
  float c = 4;

  std::vector<float> x;
  std::vector<float> y;

  for (int xx = -10; xx < 10; xx++) {
    x.push_back(xx);
    y.push_back(a * xx + c);
  }

  Polynomial<1, float> p = polynomial_regression<1, float>(x, y);

  ASSERT_FLOAT_EQ(p[0], c);
  ASSERT_FLOAT_EQ(p[1], a);
}

// Linear regression
TEST(Fit, n1_residual) {
  // y = f(x) = ax + c
  float a = 2;
  float c = 4;

  std::vector<float> x;
  std::vector<float> y;

  for (int xx = -10; xx < 10; xx++) {
    x.push_back(xx);
    y.push_back(a * xx + c);

    x.push_back(xx);
    y.push_back(a * xx + c + 1);
  }

  Polynomial<1, float> p = polynomial_regression<1, float>(x, y, true);

  ASSERT_EQ(p.data_size(), 40);

  ASSERT_FLOAT_EQ(p[0], c + 0.5); // up by half
  ASSERT_FLOAT_EQ(p[1], a);

  ASSERT_FLOAT_EQ(p.residual(), 10.0); // 40 points, 0.25 squared deviation each.
}

// Quadratic
TEST(Fit, n2) {
  // y = f(x) = ax^2 + bx + c
  float a = 2;
  float b = 3;
  float c = 4;

  std::vector<float> x;
  std::vector<float> y;

  for (int xx = -10; xx < 10; xx++) {
    x.push_back(xx);
    y.push_back(a * xx * xx + b * xx + c);
  }

  auto p = polynomial_regression<2>(x, y);

  ASSERT_FLOAT_EQ(p[0], c);
  ASSERT_FLOAT_EQ(p[1], b);
  ASSERT_FLOAT_EQ(p[2], a);
}

// Quadratic
TEST(Fit, n2_single_vector) {
  // y = f(x) = ax^2 + bx + c
  float a = 2;
  float b = 3;
  float c = 4;

  std::vector<float> y;

  for (int xx = 0; xx < 10; xx++) {
    y.push_back(a * xx * xx + b * xx + c);
  }

  auto p = polynomial_regression<2>(y);

  ASSERT_FLOAT_EQ(p[0], c);
  ASSERT_FLOAT_EQ(p[1], b);
  ASSERT_FLOAT_EQ(p[2], a);
}

// Quadratic
TEST(Fit, n2_single_vector_fixed) {
  // y = f(x) = ax^2 + bx + c
  float a = 2;
  float b = 3;
  float c = 4;

  std::vector<float> y;

  for (int xx = 0; xx < 10; xx++) {
    y.push_back(a * xx * xx + b * xx + c);
  }

  auto p = polynomial_regression_fixed<2, 10>(y);

  ASSERT_FLOAT_EQ(p[0], c);
  ASSERT_FLOAT_EQ(p[1], b);
  ASSERT_FLOAT_EQ(p[2], a);
}

// Quadratic
TEST(Fit, n2_dequeue) {
  // y = f(x) = ax^2 + bx + c
  float a = 2;
  float b = 3;
  float c = 4;

  std::deque<float> x;
  std::deque<float> y;

  for (int xx = -10; xx < 10; xx++) {
    x.push_back(xx);
    y.push_back(a * xx * xx + b * xx + c);
  }

  auto p = polynomial_regression<2>(x, y);

  ASSERT_FLOAT_EQ(p[0], c);
  ASSERT_FLOAT_EQ(p[1], b);
  ASSERT_FLOAT_EQ(p[2], a);
}

TEST(Fit, n2_dequeue_set) {
  // y = f(x) = ax^2 + bx + c
  float a = 2;
  float b = 3;
  float c = 4;

  std::set<float> x;
  std::deque<float> y;

  for (int xx = -10; xx < 10; xx++) {
    x.insert(xx);
    y.push_back(a * xx * xx + b * xx + c);
  }

  auto p = polynomial_regression<2>(x, y);

  ASSERT_FLOAT_EQ(p[0], c);
  ASSERT_FLOAT_EQ(p[1], b);
  ASSERT_FLOAT_EQ(p[2], a);
}

// Quadratic
TEST(Fit, n2_dequeue_vector) {
  // y = f(x) = ax^2 + bx + c
  float a = 2;
  float b = 3;
  float c = 4;

  std::vector<float> x;
  std::deque<float> y;

  for (int xx = -10; xx < 10; xx++) {
    x.push_back(xx);
    y.push_back(a * xx * xx + b * xx + c);
  }

  auto p = polynomial_regression<2>(x, y);

  ASSERT_FLOAT_EQ(p[0], c);
  ASSERT_FLOAT_EQ(p[1], b);
  ASSERT_FLOAT_EQ(p[2], a);
}

// Cubic with double
TEST(Fit, n3) {
  // y = f(x) = ax^3 + bx^2 + cx + d
  double a = 2;
  double b = 3;
  double c = 4;
  double d = 5;

  std::vector<double> x;
  std::vector<double> y;

  for (int xx = -20; xx < 20; xx++) {
    x.push_back(xx);
    y.push_back(a * xx * xx * xx + b * xx * xx + c * xx + d);
  }

  Polynomial<3> p = polynomial_regression<3>(x, y);

  ASSERT_FLOAT_EQ(p[0], d);
  ASSERT_FLOAT_EQ(p[1], c);
  ASSERT_FLOAT_EQ(p[2], b);
  ASSERT_FLOAT_EQ(p[3], a);
}

// Forth degree with long double
TEST(Fit, n4_long_double) {
  // y = f(x) = ax^3 + bx^2 + cx + d
  long double a = 2;
  long double b = 3;
  long double c = 4;
  long double d = 5;
  long double e = 6;

  std::vector<long double> x;
  std::vector<long double> y;

  for (int xx = -20; xx < 20; xx++) {
    x.push_back(xx);
    y.push_back(a * xx * xx * xx * xx +
                b * xx * xx * xx +
                c * xx * xx +
                d * xx +
                e);
  }

  Polynomial<4, long double, long double> p = polynomial_regression<4, long double>(x, y);

  ASSERT_FLOAT_EQ(p[0], e);
  ASSERT_FLOAT_EQ(p[1], d);
  ASSERT_FLOAT_EQ(p[2], c);
  ASSERT_FLOAT_EQ(p[3], b);
  ASSERT_FLOAT_EQ(p[4], a);
}

TEST(Fit, n4_double) {
  // y = f(x) = ax^3 + bx^2 + cx + d
  double a = 2;
  double b = 3;
  double c = 4;
  double d = 5;
  double e = 6;

  std::vector<double> x;
  std::vector<double> y;

  for (int xx = -20; xx < 20; xx++) {
    x.push_back(xx);
    y.push_back(a * xx * xx * xx * xx +
                b * xx * xx * xx +
                c * xx * xx +
                d * xx +
                e);
  }

  Polynomial<4> p = polynomial_regression<4>(x, y);

  ASSERT_FLOAT_EQ(p[0], e);
  ASSERT_FLOAT_EQ(p[1], d);
  ASSERT_FLOAT_EQ(p[2], c);
  ASSERT_FLOAT_EQ(p[3], b);
  ASSERT_FLOAT_EQ(p[4], a);
}