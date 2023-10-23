#include "model.h"

#include <iostream>

namespace s21 {

void Model::loadFromFile(const std::string& fileName) {
  std::ifstream fp(fileName);
  if (!fp.is_open()) {
    throw std::invalid_argument("Error: can't open the " + fileName);
  }
  std::string line;
  std::getline(fp, line);
  if (line != kPrefix) {
    fp.close();
    throw std::out_of_range("Error: incorrect header");
  }
  try {
    std::tm date{};
    while (!fp.eof() && std::getline(fp, line, ',')) {
      strptime(line.data(), "%Y-%m-%d", &date);
      std::getline(fp, line);
      data_points_.push_back({date, std::stod(line)});
    }
  } catch (const std::exception& e) {
    fp.close();
    throw std::out_of_range("Error: incorrect format");
  }
}

std::vector<DataPoint>& Model::getData() { return data_points_; }

void Model::showData() {
  std::cout << "Value, Data (" << data_points_.size() << " points): \n";
  for (auto it : data_points_) {
    std::cout << it.second << "\t" << std::asctime(&it.first);
  }
}

void Model::clearData() { data_points_.clear(); }

void Model::initNewtonPolynomial(const std::vector<Point>& points) {
  newton_.initNewtonPolynomial(points);
}

void Model::initNewtonPolynomial(std::vector<DataPoint>& data_points) {
  newton_.initNewtonPolynomial(data_points);
}

std::vector<double>& Model::getNewtonCoeff() { return newton_.getCoeff(); }

double Model::getNewtonValue(double t) { return newton_.getValue(t); }

void Model::initCubicSpline(const std::vector<Point>& points) {
  spline_.initCubicSpline(points);
}

void Model::initCubicSpline(std::vector<DataPoint>& data_points) {
  spline_.initCubicSpline(data_points);
}

Matrix& Model::getSplineCoeff() { return spline_.getCoeff(); }

double Model::getSplineValue(double t) { return spline_.getValue(t); }

void Model::initApproximation(const std::vector<Point>& points,
                              const int degree) {
  approx_.initApproximation(points, degree);
}

void Model::initApproximation(std::vector<DataPoint>& data_points,
                              const int degree) {
  approx_.initApproximation(data_points, degree);
}

std::vector<double>& Model::getApproxCoeff() { return approx_.getCoeff(); }

double Model::getApproxValue(double t) { return approx_.getValue(t); }

}  //   namespace s21
