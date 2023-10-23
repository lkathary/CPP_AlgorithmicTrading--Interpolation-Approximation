#ifndef SRC_MODEL_H_
#define SRC_MODEL_H_

#include <fstream>
#include <vector>

#include "Approximation/approximation.h"
#include "NewtonInterpolation/newton_interpolation.h"
#include "SplineInterpolation/spline_interpolation.h"
#include "types.h"

namespace s21 {

class Model {
 public:
  Model() {}
  ~Model() = default;
  Model(const Model &) = delete;
  Model(Model &&) = delete;
  void operator=(const Model &) = delete;
  void operator=(Model &&) = delete;

  auto loadFromFile(const std::string &filename) -> void;
  auto getData() -> std::vector<DataPoint> &;
  auto showData() -> void;
  auto clearData() -> void;

  auto initNewtonPolynomial(const std::vector<Point> &) -> void;
  auto initNewtonPolynomial(std::vector<DataPoint> &) -> void;
  auto getNewtonCoeff() -> std::vector<double> &;
  auto getNewtonValue(double t) -> double;

  auto initCubicSpline(const std::vector<Point> &) -> void;
  auto initCubicSpline(std::vector<DataPoint> &) -> void;
  auto getSplineCoeff() -> Matrix &;
  auto getSplineValue(double t) -> double;

  auto initApproximation(const std::vector<Point> &, const int) -> void;
  auto initApproximation(std::vector<DataPoint> &, const int) -> void;
  auto getApproxCoeff() -> std::vector<double> &;
  auto getApproxValue(double t) -> double;

 private:
  std::vector<DataPoint> data_points_;
  NewtonInterpolation newton_;
  SplineInterpolation spline_;
  Approximation approx_;
};

}  //   namespace s21

#endif  // SRC_MODEL_H_
