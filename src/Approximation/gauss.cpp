#include "gauss.h"

namespace s21 {

void Gauss::loadFromFile(const std::string &fileName) {
  std::ifstream fp(fileName);
  if (fp.is_open()) {
    std::string line;
    std::getline(fp, line);
    try {
      rows_ = std::stoi(line);
      cols_ = rows_ + 1;
      for (int i = 0; i < rows_; ++i) {
        std::vector<double> tmp_vector;
        for (int j = 0; j < cols_ - 1; ++j) {
          std::getline(fp, line, ' ');
          if (fp.eof()) {
            fp.close();
            throw std::out_of_range("Error: incorrect format==");
          }
          tmp_vector.push_back(std::stod(line));
        }
        std::getline(fp, line);
        tmp_vector.push_back(std::stod(line));
        matrix_.push_back(tmp_vector);
      }
    } catch (const std::exception &e) {
      fp.close();
      throw std::out_of_range("Error: incorrect format");
    }
  } else {
    throw std::invalid_argument("Error: can't open the " + fileName);
  }
}

void Gauss::toStringMatrix() {
  std::cout << "Number of equations: " << rows_ << std::endl;
  for (auto &it_h : matrix_) {
    for (auto &it_v : it_h) {
      std::cout << it_v << " ";
    }
    std::cout << std::endl;
  }
}

void Gauss::toStringResult() {
  std::cout << "Solution: [";
  if (result_.size()) {
    for (auto &it : result_) {
      std::cout << it << " ";
    }
    std::cout << "\b]" << std::endl;
  } else {
    std::cout << "]" << std::endl;
  }
}

void Gauss::clearData() {
  matrix_.clear();
  result_.clear();
  rows_ = 0;
  cols_ = 0;
}

void Gauss::updateEquation(int row, int col, int base_row) {
  // std::lock_guard<std::mutex> lock(mtx_);
  double coff = spare_matrix_[row][col] / spare_matrix_[base_row][col];
  for (int j = col; j < cols_; ++j) {
    spare_matrix_[row][j] -= spare_matrix_[base_row][j] * coff;
  }
}

void Gauss::normalizeMatrix() {
  for (int i = 0; i < rows_; ++i) {
    double coff = spare_matrix_[i][i];
    for (int j = i; j < cols_; ++j) {
      spare_matrix_[i][j] /= coff;
    }
  }
}

void Gauss::calculateGauss() {
  spare_matrix_ = matrix_;
  for (int k = 0; k < rows_ - 1; ++k) {
    for (int i = k + 1; i < rows_; ++i) {
      updateEquation(i, k, k);
    }
  }
  normalizeMatrix();
}

void Gauss::calculateGaussParallels() {
  const int num_threads = std::thread::hardware_concurrency();
  spare_matrix_ = matrix_;
  // auto job = [this](int i, int k) { updateEquation(i, k, k); };
  std::vector<std::thread> threads(num_threads);

  int cur_thread = 0;
  for (int k = 0; k < rows_ - 1; ++k) {
    for (int i = k + 1; i < rows_; ++i) {
      threads[cur_thread++] =
          std::move(std::thread{[this, i, k]() { updateEquation(i, k, k); }});
      if (cur_thread == num_threads) {
        for (auto &it : threads) {
          it.join();
        }
        cur_thread = 0;
      }
    }
    for (auto &it : threads) {
      if (it.joinable()) {
        it.join();
      }
    }
    cur_thread = 0;
  }
  normalizeMatrix();
}

void Gauss::calculateGaussParallelsExtra() {
  const int num_threads = std::thread::hardware_concurrency();
  spare_matrix_ = matrix_;

  auto job = [this](int k) {
    for (int i = k + 1; i < rows_; ++i) {
      updateEquation(i, k, k);
    }
  };
  std::vector<std::thread> threads(num_threads);

  int cur_thread = 0;
  for (int k = 0; k < rows_ - 1; ++k) {
    threads[cur_thread++] = std::move(std::thread{job, k});
    if (cur_thread == num_threads) {
      for (auto &it : threads) {
        it.join();
      }
      cur_thread = 0;
    }
    for (auto &it : threads) {
      if (it.joinable()) {
        it.join();
      }
    }
    cur_thread = 0;
  }
  normalizeMatrix();
}

std::vector<double> &Gauss::getResultWithoutParallelAlgo() {
  calculateGauss();
  result_.clear();
  for (int i = rows_ - 1; i >= 0; --i) {
    double tmp = 0;
    for (int j = 0; j < (int)result_.size(); ++j) {
      tmp += result_[j] * spare_matrix_[i][i + 1 + j];
    }
    if (std::isnan(tmp)) {
      throw std::invalid_argument("Error: incorrect SLAE matrix");
    }
    result_.insert(result_.begin(), spare_matrix_[i][cols_ - 1] - tmp);
  }
  return result_;
}

std::vector<double> &Gauss::getResultWithParallelAlgo() {
  calculateGaussParallels();
  result_.clear();
  for (int i = rows_ - 1; i >= 0; --i) {
    double tmp = 0;
    for (int j = 0; j < (int)result_.size(); ++j) {
      tmp += result_[j] * spare_matrix_[i][i + 1 + j];
    }
    if (std::isnan(tmp)) {
      throw std::invalid_argument("Error: incorrect SLAE matrix");
    }
    result_.insert(result_.begin(), spare_matrix_[i][cols_ - 1] - tmp);
  }
  return result_;
}

std::vector<double> &Gauss::getResultSLAE(const Matrix &matrix) {
  matrix_ = matrix;
  rows_ = matrix_.size();
  cols_ = rows_ + 1;

  calculateGauss();
  result_.clear();
  for (int i = rows_ - 1; i >= 0; --i) {
    double tmp = 0;
    for (int j = 0; j < (int)result_.size(); ++j) {
      tmp += result_[j] * spare_matrix_[i][i + 1 + j];
    }
    if (!std::isnan(spare_matrix_[i][cols_ - 1])) {
      result_.insert(result_.begin(), spare_matrix_[i][cols_ - 1] - tmp);
    } else {
      // throw std::invalid_argument("Error: incorrect SLAE matrix");
    }
  }
  return result_;
}

}  //  namespace s21
