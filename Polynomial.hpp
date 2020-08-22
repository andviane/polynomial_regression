#ifndef POLYNOMIAL_POLYNOMIAL_H
#define POLYNOMIAL_POLYNOMIAL_H

#include <type_traits>

namespace andviane {

// A helper class that contains coefficients of polynomial,
// providing also some trivial helper methods like interpolation.
  template<int p_order, typename TYPE=double, typename PRECISION = TYPE>
  class Polynomial {
  public:
    Polynomial(int n = 0, bool valid = true);

    Polynomial(std::array<PRECISION, p_order + 1> coefficients, bool valid = true, int n = 0);

    // Override (), allowing to use polynomial as function that interpolates
    TYPE operator()(TYPE x);

    Polynomial<p_order - 1, TYPE, PRECISION> differentiate();

    Polynomial<p_order + 1, TYPE, PRECISION> integrate(TYPE C = 0);

    // Define [] to retrieve the coefficients
    PRECISION &operator[](int a);

    // Define the begin() iterator for easy loop over polynomial variables, for (auto c: polynomial) {}
    typename std::array<PRECISION, p_order>::iterator begin();

    // Define the end() iterator for easy loop over polynomial variables
    typename std::array<PRECISION, p_order>::iterator end();

    int order();

    int data_size();

    PRECISION residual();

    void residual(PRECISION residual);

    std::string DebugString();

  private:
    std::array<PRECISION, p_order + 1> coefficients_;
    bool valid_;

    PRECISION residual_ = NAN;
    int data_size_ = 0;
  };

#include "internal/Polynomial.tpp"
}
#endif //POLYNOMIAL_POLYNOMIAL_H