#include "approximation.h"

namespace s21 {

void Approximation::initApproximation(const std::vector<Point>& points,
                                      const int degree) {
  points_ = points;
  coeff_.clear();
  calculateCoeff(degree);
}

void Approximation::initApproximation(std::vector<DataPoint>& data_points,
                                      const int degree) {
  points_.clear();
  begin = std::mktime(&(data_points.front().first));
  for (auto& it : data_points) {
    points_.push_back({std::mktime(&it.first) - begin, it.second});
  }
  coeff_.clear();
  calculateCoeff(degree);
}

std::vector<double>& Approximation::getCoeff() { return coeff_; }

double Approximation::getValue(double t) {
  if (coeff_.empty()) {
    throw std::domain_error("Error: Polynomial not inited");
  }
  double result = coeff_[0];
  for (int i = 1; i < (int)coeff_.size(); ++i) {
    result += coeff_[i] * pow((t - begin), i);
  }
  return result;
}

void Approximation::calculateCoeff(const int degree) {
  if (!points_.empty()) {
    calculateMatrixSLAE(degree);
    Gauss gauss;
    try {
      coeff_ = gauss.getResultSLAE(slae_coeff_);
    } catch (const std::exception& e) {
      coeff_.clear();
      std::cerr << e.what() << std::endl;
    }
  }
}

void Approximation::calculateMatrixSLAE(const int degree) {
  slae_coeff_.clear();
  slae_coeff_.resize(degree + 1);  // rows
  for (auto& it : slae_coeff_) {
    it.resize(degree + 2);  // cols
  }
  for (int i = 0; i < degree + 1; ++i) {
    for (int j = i; j < degree + 1; ++j) {
      slae_coeff_[i][j] = slae_coeff_[j][i] = calcSum(i + j);
    }
  }
  for (int i = 0; i < degree + 1; ++i) {
    slae_coeff_[i][degree + 1] = 0;
    for (int j = 0; j < (int)points_.size(); ++j) {
      slae_coeff_[i][degree + 1] +=
          points_[j].second * pow(points_[j].first, i);
    }
  }
}

double Approximation::calcSum(const int degree) {
  double result = 0;
  for (int i = 0; i < (int)points_.size(); ++i) {
    result += pow(points_[i].first, degree);
  }
  return result;
}

}  //   namespace s21
