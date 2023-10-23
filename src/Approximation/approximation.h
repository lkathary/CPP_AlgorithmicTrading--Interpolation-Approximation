#ifndef SRC_APPROXIMATION_APPROXIMATION_H_
#define SRC_APPROXIMATION_APPROXIMATION_H_

//
// Based on
// http://simenergy.ru/mathematical-analysis/digital-processing/ordinary-least-squares
//

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "../types.h"
#include "gauss.h"

namespace s21 {

class Approximation {
 public:
  Approximation() {}
  ~Approximation() = default;
  Approximation(const Approximation&) = delete;
  Approximation(Approximation&&) = delete;
  void operator=(const Approximation&) = delete;
  void operator=(Approximation&&) = delete;

  auto initApproximation(const std::vector<Point>&, const int degree) -> void;
  auto initApproximation(std::vector<DataPoint>&, const int degree) -> void;

  auto getCoeff() -> std::vector<double>&;
  auto getValue(double t) -> double;

 private:
  auto calculateCoeff(const int degree) -> void;
  auto calculateMatrixSLAE(const int degree) -> void;
  auto calcSum(const int degree) -> double;

  Matrix slae_coeff_{};
  std::vector<double> coeff_{};
  std::vector<Point> points_{};
  double begin{};
};

}  //   namespace s21

#endif  //  SRC_APPROXIMATION_APPROXIMATION_H_
