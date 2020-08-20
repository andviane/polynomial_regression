#ifndef _POLYNOMIAL_REGRESSION_ABC_H
#define _POLYNOMIAL_REGRESSION_ABC_H  __POLYNOMIAL_REGRESSION_ABC_H

/**
 * PURPOSE:
 *
 *  Polynomial Regression aims to fit a non-linear relationship to a set of
 *  points. It approximates this by solving a series of linear equations using
 *  a least-squares approach.
 *
 *  We can model the expected value y as an nth degree polynomial, yielding
 *  the general polynomial regression model:
 *
 *  y = a0 + a1 * x + a2 * x^2 + ... + an * x^n
 *
 * LICENSE:
 *
 * MIT License
 *
 * Copyright (c) 2020 Chris Engelsma, Audrius Meskauskas
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * @author Chris Engelsma (initial version, all algorithm)
 * @author Audrius Meskauskas (later changes starting from August 19, 2020)
 */

#include <vector>
#include <cstdlib>
#include <stdexcept>
#include <cmath>
#include <cassert>
#include <array>
#include <string.h>
#include "Polynomial.hpp"
#include "internal/polynomial_regression_internals.hpp"

// Perform polynomial regression using X and Y iterators
template<int n, typename TYPE=double, typename PRECISION=TYPE, typename ITERATOR_X, typename ITERATOR_Y>
Polynomial<n, TYPE, PRECISION> polynomial_regression(ITERATOR_X x_iter,
                                                     ITERATOR_Y y_iter,
                                                     size_t N, bool compute_residual = false) {
  static_assert(n >= 0);

  // X values raised in degree.
  std::array<std::vector<PRECISION>, 2 * n + 1> x_raised;
  build_x_matrix<2 * n + 1, PRECISION, ITERATOR_X>(x_iter, N, x_raised);
  return polynomial_regression<n, TYPE, PRECISION, ITERATOR_Y>(x_raised, y_iter, N, compute_residual);
}

// Perform polynomial regression Y iterator only (X enumerates 0 to N)
template<int n, typename TYPE=double, typename PRECISION=TYPE, typename ITERATOR_Y>
Polynomial<n, TYPE, PRECISION> polynomial_regression(ITERATOR_Y y_iter,
                                                     size_t N, bool compute_residual = false) {
  static_assert(n >= 0);

  // X values raised in degree.
  std::array<std::vector<PRECISION>, 2 * n + 1> x_raised;
  build_x_matrix<2 * n + 1, PRECISION>(N, x_raised);
  return polynomial_regression<n, TYPE, PRECISION, ITERATOR_Y>(x_raised, y_iter, N, compute_residual);
}

// Perform polynomial regression using Y iterator only (X enumerates 0 to N assuming the fixed sample size)
template<int n, int fixed_size, typename TYPE=double, typename PRECISION=TYPE, typename ITERATOR_Y>
Polynomial<n, TYPE, PRECISION> polynomial_regression(ITERATOR_Y y_iter, bool compute_residual = false) {
  static_assert(n >= 0);

  // X values raised in degree, static to compute oly once
  static std::array<std::vector<PRECISION>, 2 * n + 1> x_raised;
  if (x_raised[0].empty()) {
    // Build once on the first call.
    build_x_matrix<2 * n + 1, PRECISION>(fixed_size, x_raised);
  }

  return polynomial_regression<n, TYPE, PRECISION, ITERATOR_Y>(x_raised, y_iter, fixed_size, compute_residual);
}

// Perform polynomial regression over two collections that may have different type but expecting the same size
template<int order, typename TYPE=double, typename PRECISION=TYPE,
    typename COLLECTION_X=std::vector<TYPE>, typename COLLECTION_Y=std::vector<TYPE>>
Polynomial<order, TYPE, PRECISION> polynomial_regression(const COLLECTION_X &x,
                                                         const COLLECTION_Y &y, bool compute_residual = false) {
  assert(x.size() == y.size());
  assert(x.size() > 0);
  return polynomial_regression<order, TYPE, PRECISION>(x.begin(), y.begin(), x.size(), compute_residual);
}

// Perform polynomial regression over single collection (x simply changes 0 to N)
template<int order, typename TYPE=double, typename PRECISION=TYPE, typename COLLECTION_Y=std::vector<TYPE>>
Polynomial<order, TYPE, PRECISION> polynomial_regression(const COLLECTION_Y &y, bool compute_residual = false) {
  assert(y.size() > 0);
  return polynomial_regression<order, TYPE, PRECISION>(y.begin(), y.size(), compute_residual);
}

// Perform polynomial regression over single collection assuming the fixed sample size (x simply changes 0 to N)
// Assuming fixed size allows to compute the x_raised matrix only once.
template<int order, int fixed_size, typename TYPE=double, typename PRECISION=TYPE, typename COLLECTION_Y=std::vector<TYPE>>
Polynomial<order, TYPE, PRECISION> polynomial_regression_fixed(const COLLECTION_Y &y, bool compute_residual = false) {
  assert(y.size() <= fixed_size);
  return polynomial_regression<order, fixed_size, TYPE, PRECISION>(y.begin(), compute_residual);
}

#endif
