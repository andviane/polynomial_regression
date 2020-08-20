#include "gtest/gtest.h"

#include "polynomial_regression.hpp"

// Quadratic double (no need for a type anywhere)
TEST(Interpolate, n2) {
  // y = f(x) = ax^2 + bx + c
  double a = 2;
  double b = 3;
  double c = 4;

  std::vector<double> x;
  std::vector<double> y;

  for (int xx = 0; xx < 10; xx++) {
    x.push_back(xx);
    y.push_back(a * xx * xx + b * xx + c);
  }

  Polynomial<2> polynomial = polynomial_regression<2>(x, y);

  for (int xx = 0; xx < 10; xx++) {
    ASSERT_FLOAT_EQ(polynomial(xx), y[xx]);
  }
}

// Quadratic
TEST(Interpolate, n2_float) {
  // y = f(x) = ax^2 + bx + c
  float a = 2;
  float b = 3;
  float c = 4;

  std::vector<float> x;
  std::vector<float> y;

  for (int xx = 0; xx < 10; xx++) {
    x.push_back(xx);
    y.push_back(a * xx * xx + b * xx + c);
  }

  // We need internal precision of 'double' for sufficient accuracy
  Polynomial<2, float, double> polynomial = polynomial_regression<2, float, double>(x, y);

  for (int xx = 0; xx < 10; xx++) {
    ASSERT_FLOAT_EQ(polynomial(xx), y[xx]);
  }
}

// This test that just works with uint8_t on the surface. The machinery behind stays double
// so the interpolation stays precise.
TEST(Interpolate, n2_int) {
  // y = f(x) = ax^2 + bx + c
  uint8_t a = 2;
  uint8_t b = 3;
  uint8_t c = 4;

  std::vector<uint8_t> x;
  std::vector<uint8_t> y;

  for (uint8_t xx = 0; xx < 10; xx++) {
    x.push_back(xx);
    y.push_back(a * xx * xx + b * xx + c);
  }

  // We need internal precision of 'double' for sufficient accuracy
  auto polynomial = polynomial_regression<2, uint8_t, double>(x, y);

  for (uint8_t xx = 0; xx < 10; xx++) {
    ASSERT_EQ(polynomial(xx), y[xx]);
  }
}

// High degree
TEST(Interpolate, n8_float) {
  constexpr int degree = 8;
  std::vector<float> coeffs;
  for (int i = degree; i > 0; i--) {
    coeffs.push_back(3 + 2 * i);
  }

  std::vector<float> x;
  std::vector<float> y;

  for (int xx = 0; xx < 10; xx++) {
    x.push_back(xx);
    long double s = 0;
    long double xxx = 1;
    for (int d = 0; d < coeffs.size(); d++) {
      s = s + xxx * coeffs[d];
      xxx = xxx * xx;
    }
    y.push_back(s);
  }

  Polynomial<degree, float, long double> polynomial = polynomial_regression<degree, float, long double>(x, y);

  for (int xx = 0; xx < 10; xx++) {
    ASSERT_FLOAT_EQ(polynomial(xx), y[xx]);
  }
}

// Using 128 bit float if such available
#ifdef  __SIZEOF_FLOAT128__
TEST(Interpolate, n64_float128) {
  constexpr int degree = 64;

  std::vector<float> x;
  std::vector<float> y;

  // Interpolating sin(x)
  for (float xx = -M_PI; xx < M_PI; xx = xx + 0.001) {
    x.push_back(xx);
    y.push_back(std::sin(xx));
  }

  Polynomial<degree, float, __float128> polynomial = polynomial_regression<degree, float, __float128>(x, y);

  for (int i = 0; i < x.size(); i++) {
    ASSERT_NEAR(polynomial(x[i]), y[i], 1E-7);
  }
}
#endif


