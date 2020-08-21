#include "gtest/gtest.h"

#include "polynomial_regression.hpp"

// Differentiation and integration tests

using namespace andviane;

Polynomial<2> make_polynomial() {
  // y = f(x) = ax^2 + bx + c
  double a = 2;
  double b = 3;
  double c = 4;

  std::vector<double> x;
  std::vector<double> y;

  for (double xx = -10; xx < 10; xx++) {
    x.push_back(xx);
    y.push_back(a * xx * xx + b * xx + c);
  }

  return polynomial_regression<2>(x, y);
}

TEST(Differentiate, n2) {
  Polynomial p = make_polynomial();
  Polynomial derivative = p.differentiate();

  ASSERT_EQ(derivative.order(), p.order() - 1);
  ASSERT_FLOAT_EQ(derivative[0], p[1]);
  ASSERT_FLOAT_EQ(derivative[1], 2 * p[2]);

  ASSERT_FLOAT_EQ(derivative(0.5), 5);
}

TEST(Integrate, n2) {
  Polynomial p = make_polynomial();
  Polynomial integral = p.integrate();

  ASSERT_EQ(integral.order(), p.order() + 1);

  ASSERT_FLOAT_EQ(integral[0], 0);
  ASSERT_FLOAT_EQ(integral[1], p[0]);
  ASSERT_FLOAT_EQ(integral[2], p[1]/2.0);
  ASSERT_FLOAT_EQ(integral[3], p[2]/3.0);

  ASSERT_FLOAT_EQ(integral(0.5), 2.458333333);
}