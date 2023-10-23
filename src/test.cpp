#include <gtest/gtest.h>

#include "controller.h"

const std::string kDataSet = "./datasets/";

static s21::Model model;

TEST(model, LoadFromFile_ShowData) {
  s21::Controller& ctrl = s21::Controller::GetInstance();
  ctrl.Connect(&model);

  ctrl.LoadFromFile(kDataSet + "x2.csv");
  ctrl.ShowData();

  ctrl.Clear();
  ctrl.ShowData();

  ASSERT_EQ(4, 4);
}

TEST(model, GetNewtonCoeff_1) {
  s21::Controller& ctrl = s21::Controller::GetInstance();
  ctrl.Connect(&model);

  std::vector<s21::Point> points{{1, 2}, {2, 3}, {4, 1}, {7, 4}};
  ctrl.initNewtonPolynomial(points);

  auto result = ctrl.GetNewtonCoeff();
  for (auto& it : result) {
    std::cout << it << std::endl;
  }

  double t = 3.0;
  std::cout << "F(" << t << ") = " << ctrl.GetNewtonValue(t) << std::endl;
  ASSERT_NEAR(ctrl.GetNewtonValue(t), 2.311111, 1e-6);

  for (auto& it : points) {
    ASSERT_EQ(ctrl.GetNewtonValue(it.first), it.second);
  }
}

TEST(model, GetNewtonCoeff_2) {
  s21::Controller& ctrl = s21::Controller::GetInstance();
  ctrl.Connect(&model);

  std::vector<s21::Point> points{{1, 0}, {2, 6}, {3, 28}, {4, 96}, {5, 252}};
  std::vector<double> sample_coeff{0, 6, 8, 5, 0.5};
  std::vector<s21::Point> check_points{{1, 0},    {2, 6},   {3, 28},
                                       {4, 96},   {5, 252}, {6, 550},
                                       {7, 1056}, {8, 1848}};

  ctrl.initNewtonPolynomial(points);

  auto result = ctrl.GetNewtonCoeff();
  for (int i = 0; i < (int)result.size(); ++i) {
    ASSERT_EQ(sample_coeff[i], result[i]);
  }

  for (auto& it : check_points) {
    ASSERT_EQ(ctrl.GetNewtonValue(it.first), it.second);
  }
}

TEST(model, GetSplineCoeff_1) {
  s21::Controller& ctrl = s21::Controller::GetInstance();
  ctrl.Connect(&model);

  std::vector<s21::Point> points{{1, 2}, {2, 3}, {4, 1}, {7, 4}};

  ctrl.initCubicSpline(points);
  auto result = ctrl.GetSplineCoeff();
  for (auto& it_h : result) {
    for (auto& it_v : it_h) {
      std::cout << it_v << " ";
    }
    std::cout << std::endl;
  }
  for (auto& it : points) {
    ASSERT_DOUBLE_EQ(ctrl.GetSplineValue(it.first), it.second);
  }

  double t = 3.0;
  std::cout << "F(" << t << ") = " << ctrl.GetSplineValue(t) << std::endl;
  ASSERT_NEAR(ctrl.GetSplineValue(t), 2.173527, 1e-6);
}

TEST(model, GetSplineCoeff_2) {
  s21::Controller& ctrl = s21::Controller::GetInstance();
  ctrl.Connect(&model);

  std::vector<s21::Point> points{
      {1, 2}, {1.5, 3}, {2.0, 2.5}, {2.5, 4}, {3, 4.5}};

  ctrl.initCubicSpline(points);
  auto result = ctrl.GetSplineCoeff();
  for (auto& it_h : result) {
    for (auto& it_v : it_h) {
      std::cout << it_v << " ";
    }
    std::cout << std::endl;
  }
  for (auto& it : points) {
    ASSERT_DOUBLE_EQ(ctrl.GetSplineValue(it.first), it.second);
  }

  double t = 2.7;
  std::cout << "F(" << t << ") = " << ctrl.GetSplineValue(t) << std::endl;
  ASSERT_NEAR(ctrl.GetSplineValue(t), 4.41089, 1e-6);
}

TEST(model, GetApproxCoeff_1) {
  s21::Controller& ctrl = s21::Controller::GetInstance();
  ctrl.Connect(&model);

  std::vector<s21::Point> points{{0, 4}, {1, 1}, {2, 0}, {3, 1}, {4, 4}};
  // Parabola

  ctrl.initApproximation(points, 1);  //  line
  auto result = ctrl.GetApproxCoeff();
  std::cout << "line:" << std::endl;
  for (auto& it : result) {
    std::cout << it << std::endl;
  }
  for (auto& it : points) {
    ASSERT_DOUBLE_EQ(ctrl.GetApproxValue(it.first), 2);
  }

  ctrl.initApproximation(points, 2);  // parabola
  result = ctrl.GetApproxCoeff();
  std::cout << "parabola:" << std::endl;
  for (auto& it : result) {
    std::cout << it << std::endl;
  }
  for (auto& it : points) {
    ASSERT_DOUBLE_EQ(ctrl.GetApproxValue(it.first), it.second);
  }
}

int main(int argc, char* argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
