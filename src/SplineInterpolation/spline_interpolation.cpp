#include "spline_interpolation.h"

namespace s21 {

void SplineInterpolation::resetCoeff(size_t number) {
  for (auto& it : coeff_) {
    it.clear();
  }
  coeff_.resize(number);
  for (auto& it : coeff_) {
    it.resize(4);
  }
}

void SplineInterpolation::initCubicSpline(const std::vector<Point>& points) {
  points_ = points;
  resetCoeff(points.size());
  calculateCoeff();
}

void SplineInterpolation::initCubicSpline(std::vector<DataPoint>& data_points) {
  points_.clear();
  for (auto& it : data_points) {
    points_.push_back({std::mktime(&it.first), it.second});
  }
  resetCoeff(data_points.size());
  calculateCoeff();
}

Matrix& SplineInterpolation::getCoeff() { return coeff_; }

double SplineInterpolation::getValue(double t) {
  if (coeff_.empty()) {
    throw std::domain_error("Error: Spline polynomial not inited");
  }
  return calculateValue(t);
}

void SplineInterpolation::calculateCoeff() {
  if (!points_.empty()) {
    size_t size = points_.size() - 1;

    for (size_t i = 0; i <= size; ++i) {
      coeff_[i][0] = points_[i].second;
    }
    coeff_[0][2] = 0;

    double a{0}, f{0}, c{0};
    std::vector<double> alpha(size, 0);
    std::vector<double> beta(size, 0);
    for (size_t i = 1; i < size; ++i) {
      double b{0}, z{0};
      a = points_[i].first - points_[i - 1].first;
      b = points_[i + 1].first - points_[i].first;
      c = 2.0 * (points_[i + 1].first - points_[i - 1].first);
      f = 6.0 * ((points_[i + 1].second - points_[i].second) / b -
                 (points_[i].second - points_[i - 1].second) / a);
      z = a * alpha[i - 1] + c;
      alpha[i] = -b / z;
      beta[i] = (f - a * beta[i - 1]) / z;
    }

    coeff_[size][2] =
        (f - a * beta[size - 1]) / (c + a * alpha[size - 1]) / 2.0;
    for (size_t i = size - 1; i > 0; --i) {
      coeff_[i][2] = (alpha[i] * coeff_[i + 1][2] + beta[i]) / 2.0;
    }

    for (size_t i = size; i > 0; --i) {
      double h = points_[i].first - points_[i - 1].first;
      coeff_[i][3] = (coeff_[i][2] - coeff_[i - 1][2]) / 3.0 / h;
      coeff_[i][1] = (2.0 * coeff_[i][2] + coeff_[i - 1][2]) * h / 3.0 +
                     (coeff_[i][0] - coeff_[i - 1][0]) / h;
    }
  }
}

double SplineInterpolation::calculateValue(double t) {
  for (size_t i = 1; i < points_.size(); ++i) {
    if ((points_[i - 1].first - s21::kEps) < t &&
        (points_[i].first + s21::kEps) > t) {
      t -= points_[i].first;
      return coeff_[i][0] +
             t * (coeff_[i][1] + coeff_[i][2] * t + coeff_[i][3] * t * t);
    }
  }
  throw std::invalid_argument("Argument is out of range");
}

}  //   namespace s21
