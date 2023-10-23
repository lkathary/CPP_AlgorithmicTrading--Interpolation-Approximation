#ifndef SRC_NEWTONINTERPOLATION_NEWTON_INTERPOLATION_H_
#define SRC_NEWTONINTERPOLATION_NEWTON_INTERPOLATION_H_

#include <iostream>
#include <stdexcept>
#include <vector>

#include "../types.h"

namespace s21 {

class NewtonInterpolation {
 public:
  NewtonInterpolation() {}
  ~NewtonInterpolation() = default;
  NewtonInterpolation(const NewtonInterpolation&) = delete;
  NewtonInterpolation(NewtonInterpolation&&) = delete;
  void operator=(const NewtonInterpolation&) = delete;
  void operator=(NewtonInterpolation&&) = delete;

  auto initNewtonPolynomial(const std::vector<Point>&) -> void;
  auto initNewtonPolynomial(std::vector<DataPoint>&) -> void;

  auto getCoeff() -> std::vector<double>&;
  auto getValue(double t) -> double;

 private:
  auto calculateCoeff() -> void;
  auto calculateValue(size_t degree, double t) -> double;

  std::vector<double> coeff_{};
  std::vector<Point> points_{};
};

}  //   namespace s21

#endif  //  SRC_NEWTONINTERPOLATION_NEWTON_INTERPOLATION_H_
