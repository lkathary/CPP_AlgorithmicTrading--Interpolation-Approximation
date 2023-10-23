#ifndef SRC_APPROXIMATION_GAUSS_H_
#define SRC_APPROXIMATION_GAUSS_H_

#include <cmath>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

namespace s21 {

using Matrix = std::vector<std::vector<double>>;

class Gauss {
 public:
  Gauss() = default;
  ~Gauss() = default;

  auto loadFromFile(const std::string &filename) -> void;
  auto clearData() -> void;
  auto toStringMatrix() -> void;
  auto toStringResult() -> void;

  auto getResultWithoutParallelAlgo() -> std::vector<double> &;
  auto getResultWithParallelAlgo() -> std::vector<double> &;
  auto getResultSLAE(const Matrix &matrix) -> std::vector<double> &;

  Matrix &getMatrix() { return matrix_; }
  Matrix &getSpareMatrix() { return spare_matrix_; }

 private:
  auto calculateGauss() -> void;
  auto calculateGaussParallels() -> void;
  auto calculateGaussParallelsExtra() -> void;
  auto updateEquation(int row, int col, int base_row) -> void;
  auto normalizeMatrix() -> void;

  Matrix matrix_;
  Matrix spare_matrix_;
  std::vector<double> result_;
  int cols_{0};
  int rows_{0};
  std::mutex mtx_;
};

}  //  namespace s21

#endif  //  SRC_APPROXIMATION_GAUSS_H_
