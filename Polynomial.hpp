#ifndef POLYNOMIAL_POLYNOMIAL_H
#define POLYNOMIAL_POLYNOMIAL_H

#include <type_traits>

// A helper class that contains coefficients of polynomial,
// providing also some trivial helper methods like interpolation.
template<int p_order, typename TYPE=double, typename PRECISION = TYPE>
class Polynomial {
public:
  Polynomial(int n = 0, bool valid = true) : valid_(valid), data_size_(n) {
    static_assert(p_order >= 0);
    coefficients_.fill(0);
  }

  Polynomial(std::array<PRECISION, p_order + 1> coefficients, bool valid = true, int n = 0) :
      valid_(valid), data_size_(n) {
    static_assert(p_order >= 0);
    coefficients_ = coefficients;
  }

  // Override (), allowing to use polynomial as function that interpolates
  TYPE operator()(TYPE x) {
    PRECISION xx = 1;
    PRECISION s = 0;
    for (PRECISION a: coefficients_) {
      s = s + xx * a;
      xx = xx * x;
    }

    // If the "official type" happens to be integer or the like, we need a proper rounding.
    return std::is_integral<TYPE>::value ? (TYPE) std::round( (double) s) : (TYPE) s;
  }

  Polynomial<p_order - 1, TYPE, PRECISION> differentiate() {
    static_assert(p_order > 0);
    Polynomial<p_order - 1, TYPE, PRECISION> diff;
    for (int n = 1; n <= p_order; n++) {
      diff[n - 1] = n * coefficients_[n];
    }
    return diff;
  }

  Polynomial<p_order + 1, TYPE, PRECISION> integrate(TYPE C = 0) {
    Polynomial<p_order + 1, TYPE, PRECISION> integ;
    for (int n = 1; n <= p_order + 1; n++) {
      integ[n] = coefficients_[n - 1] / (PRECISION) n;
    }
    integ[0] = C;
    return integ;
  }

  // Define [] to retrieve the coefficients
  PRECISION &operator[](int a) {
    return coefficients_.at(a);
  }

  // Define the iterators for easy loop
  typename std::array<PRECISION, p_order>::iterator begin() {
    return coefficients_.begin();
  }

  typename std::array<PRECISION, p_order>::iterator end() {
    return coefficients_.end();
  }

  int order() {
    return p_order;
  }

  int data_size() {
    return data_size_;
  }

  PRECISION residual() {
    return residual_;
  }

  void residual(PRECISION residual) {
    residual_ = residual;
  }

  std::string DebugString() {
    std::string expression;
    for (int n = p_order; n >= 0; n--) {
      TYPE k = coefficients_.at(n);
      switch (n) {
        case 0:
          expression = expression + std::to_string(k);
          break;
        case 1:
          expression = expression + std::to_string(k) + " * x + ";
          break;
        default:
          expression = expression + std::to_string(k) + " * x^" + std::to_string(n) + " + ";
          break;
      }
    }
    return expression;
  }

private:
  std::array<PRECISION, p_order + 1> coefficients_;
  bool valid_;

  PRECISION residual_ = NAN;
  int data_size_ = 0;
};

#endif //POLYNOMIAL_POLYNOMIAL_H