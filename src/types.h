#ifndef SRC_TYPES_H_
#define SRC_TYPES_H_

#include <ctime>

namespace s21 {

const std::string kPrefix = "Date,Close";

constexpr int kSecInDay = 86400;
constexpr double kPadCoeff = 0.05;
constexpr int kMaxCountGraph = 5;
constexpr double kEps = 1e-6;

using DataPoint = std::pair<std::tm, double>;
using Point = std::pair<double, double>;
using Matrix = std::vector<std::vector<double>>;

}  //  namespace s21

#endif  //  SRC_TYPES_H_
