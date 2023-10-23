#include "newton_interpolation.h"

namespace s21 {

void NewtonInterpolation::initNewtonPolynomial(
    const std::vector<Point>& points) {
  coeff_.clear();
  points_ = points;
  calculateCoeff();
}

void NewtonInterpolation::initNewtonPolynomial(
    std::vector<DataPoint>& data_points_) {
  coeff_.clear();
  points_.clear();
  for (auto& it : data_points_) {
    points_.push_back({std::mktime(&it.first), it.second});
  }
  calculateCoeff();
}

std::vector<double>& NewtonInterpolation::getCoeff() { return coeff_; }

double NewtonInterpolation::getValue(double t) {
  if (coeff_.empty()) {
    throw std::domain_error("Error: Newton polynomial not inited");
  }
  return calculateValue(points_.size() - 1, t);
}

void NewtonInterpolation::calculateCoeff() {
  if (!points_.empty()) {
    coeff_.push_back(points_[0].second);
    for (size_t i = 1; i < points_.size(); ++i) {
      double p = 1.0;
      for (size_t j = 0; j < i; ++j) {
        p *= points_[i].first - points_[j].first;
      }
      coeff_.push_back(
          (points_[i].second - calculateValue(i - 1, points_[i].first)) / p);
    }
  }
}

double NewtonInterpolation::calculateValue(size_t degree, double t) {
  double p = 1.0;
  double sum = coeff_[0];
  for (size_t i = 1; i <= degree; ++i) {
    p *= t - points_[i - 1].first;
    sum += coeff_[i] * p;
  }
  return sum;
}

}  //   namespace s21
