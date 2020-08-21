#include <iostream>
#include <vector>
#include "../polynomial_regression.hpp"

int main() {
  // We need data sample first
  // y = f(x) = ax^2 + bx + c

  float a = 2;
  float b = 3;
  float c = 4;

  std::vector<float> x;
  std::vector<float> y;

  for (float xx = 0; xx < 10; xx++) {
    x.push_back(xx);
    y.push_back(a * xx * xx + b * xx + c);
  }

  // Compute, differentiate and integrate
  auto polynomial = andviane::polynomial_regression<2>(x, y);  // second degree polynomial

  std::cout << "Polynomial " << polynomial.DebugString() << std::endl;
  std::cout << "f(0.5) = " << polynomial(0.5) << std::endl // 6.0
            << "f'(0.5) = " << polynomial.differentiate()(0.5) << std::endl
            << "S f(0.5) = " << polynomial.integrate()(0.5) << std::endl;

  // Do not care about x ? Assume it as 0 .. n
  auto simple = andviane::polynomial_regression<2>(y);
  std::cout << "Polynomial with auto-x " << simple.DebugString() << std::endl;
  std::cout << "f(0.5) = " << simple(0.5) << std::endl; // 6.0

  // Also fixed sample size ? Make a parameter for performance.
  auto simple_fixed = andviane::polynomial_regression_fixed<2, 10>(y); // more is ok, just not less
  std::cout << "Polynomial with auto-x and fixed " << simple_fixed.DebugString() << std::endl;
  std::cout << "f(0.5) = " << simple_fixed(0.5) << std::endl; // 6.0

  float xx = 0.5;
  std::cout << "Really: " << a * xx * xx + b * xx + c << std::endl;

  return 0;
}