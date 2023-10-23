#ifndef SRC_SPLINEINTERPOLATION_SPLINE_INTERPOLATION_H_
#define SRC_SPLINEINTERPOLATION_SPLINE_INTERPOLATION_H_

#include <iostream>
#include <stdexcept>
#include <vector>

#include "../types.h"

namespace s21 {

class SplineInterpolation {
 public:
  SplineInterpolation() {}
  ~SplineInterpolation() = default;
  SplineInterpolation(const SplineInterpolation&) = delete;
  SplineInterpolation(SplineInterpolation&&) = delete;
  void operator=(const SplineInterpolation&) = delete;
  void operator=(SplineInterpolation&&) = delete;

  auto initCubicSpline(const std::vector<Point>&) -> void;
  auto initCubicSpline(std::vector<DataPoint>&) -> void;

  auto getCoeff() -> Matrix&;
  auto getValue(double t) -> double;

 private:
  auto resetCoeff(size_t number) -> void;
  auto calculateCoeff() -> void;
  auto calculateValue(double t) -> double;

  Matrix coeff_{};
  std::vector<Point> points_{};
};

}  //   namespace s21

#endif  //  SRC_SPLINEINTERPOLATION_SPLINE_INTERPOLATION_H_
