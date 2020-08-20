This project is derived from 
[Chris Engelsma gist](https://gist.github.com/chrisengelsma/108f7ab0a746323beaaf7d6634cf4add). It was available under MIT license that stays. The goal of this derived work is to wrap the provided
algorithm somewhat "more seriously": add unit tests and make it project, not a gist. There are
also some extensions:

- The class `Polynomial`, returned by the regression, not just holds coefficients but 
  can also be used as a function for interpolation. Residual can optionally be provided.
- The data type for internal calculations is defined separately from the type of X and Y. You can
  use `uint_8_t` for X and Y and still apply a 8th degree polynomial with all fitting done using `long double` instead. 
  `__float128` may be tried if available. The test suite contains the 64th degree regression over about 6000 values.
- It is possible to use various STL containers like `std::deque` for interpolation, or iterators. 
  The choice is no longer restricted to `std::vector`.
- It is possible to supply only Y values (X values are inferred as [ 0 .. Y.size() [ ). This should work well with
  the the fixed rate sampling.
- The Polynomial class can also return derivative or integral of itself.

In comparison to the initial code, there are the following optimizations:
- This X and Y values are picked only once via sequential iterators. This makes no difference for an array,
  but there are classes like list or deque that do not provide efficient random access.
- The function `std::pow` is no longer used. This should allow the usage of custom high precision data types,
  as long as they implement the basic arithmetic operators.   
- The polynomial degree is made fixed (it is a parameter of template). This allows to use `std::array` that is
  more efficient and cache friendly class that `std::vector`. 
- If X is not present, the number of the sampled values can also be fixed. This allows to pre-compute and cache
  a large matrix of x'es raised in degrees up till `2*n + 1` that is required for the algorithm.    

While maybe overkill, this library can also do a linear regression for you. It is just a polynomial regression
of the degree 1.


It it a standard CMake project that should be simple to build:

```
mkdir build
cd build
cmake ..
make
```

The current minimums are gcc 8.4.0, cmake 3.16.0 and gtest 1.10.0 
for the test suite.

Currently it builds a static library, libpolynomial_regression.a. 

This library is also easy to use:

```
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
  auto polynomial = polynomial_regression<2>(x, y);  // second degree polynomial

  std::cout << "Polynomial " << polynomial.DebugString() << std::endl;
  std::cout << "f(0.5) = " << polynomial(0.5) << std::endl // 6.0
            << "f'(0.5) = " << polynomial.differentiate()(0.5) << std::endl
            << "S f(0.5) = " << polynomial.integrate()(0.5) << std::endl;

  // Do not care about x ? Assume it as 0 .. n
  auto simple = polynomial_regression<2>(y);
  std::cout << "Polynomial with auto-x " << simple.DebugString() << std::endl;
  std::cout << "f(0.5) = " << simple(0.5) << std::endl; // 6.0

  // Also fixed sample size ? Make a parameter for performance.
  auto simple_fixed = polynomial_regression_fixed<2, 10>(y); // more is ok, just not less
  std::cout << "Polynomial with auto-x and fixed " << simple_fixed.DebugString() << std::endl;
  std::cout << "f(0.5) = " << simple_fixed(0.5) << std::endl; // 6.0
```  


   
