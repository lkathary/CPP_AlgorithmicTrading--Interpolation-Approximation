#ifndef SRC_CONTROLLER_H_
#define SRC_CONTROLLER_H_

#include "model.h"

namespace s21 {

class Controller {
 public:
  static Controller& GetInstance() {
    static Controller controller_;
    return controller_;
  }

  void Connect(s21::Model* model) { model_ = model; }
  ~Controller() = default;

  std::string LoadFromFile(const std::string& file) {
    try {
      model_->loadFromFile(file);
      return "Data loaded successfully from " + file;
    } catch (const std::exception& e) {
      Clear();
      return e.what();
    }
  }

  std::vector<DataPoint>& GetData() { return model_->getData(); }
  void ShowData() { model_->showData(); }
  void Clear() { model_->clearData(); }

  void initNewtonPolynomial(const std::vector<Point>& points) {
    model_->initNewtonPolynomial(points);
  }
  void initNewtonPolynomial(std::vector<DataPoint>& data_points) {
    model_->initNewtonPolynomial(data_points);
  }
  std::vector<double>& GetNewtonCoeff() { return model_->getNewtonCoeff(); }
  double GetNewtonValue(double t) { return model_->getNewtonValue(t); }

  void initCubicSpline(const std::vector<Point>& points) {
    model_->initCubicSpline(points);
  }
  void initCubicSpline(std::vector<DataPoint>& data_points) {
    model_->initCubicSpline(data_points);
  }
  Matrix& GetSplineCoeff() { return model_->getSplineCoeff(); }
  double GetSplineValue(double t) { return model_->getSplineValue(t); }

  void initApproximation(const std::vector<Point>& points, const int degree) {
    model_->initApproximation(points, degree);
  }
  void initApproximation(std::vector<DataPoint>& data_points,
                         const int degree) {
    model_->initApproximation(data_points, degree);
  }
  std::vector<double>& GetApproxCoeff() { return model_->getApproxCoeff(); }
  double GetApproxValue(double t) { return model_->getApproxValue(t); }

 private:
  s21::Model* model_;

  Controller() = default;
};

}  //   namespace s21

#endif  //   SRC_CONTROLLER_H_
