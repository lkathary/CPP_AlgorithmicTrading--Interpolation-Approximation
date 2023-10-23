#ifndef SRC_MAINWINDOW_H_
#define SRC_MAINWINDOW_H_

#include <QMainWindow>

#include "controller.h"
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

 private slots:
  void on_pushButtonInfo_clicked();
  void on_pushButtonClear_clicked();
  void on_pushButtonOpen_clicked();
  void on_pushButtonCleanData_clicked();
  void on_pushButtonReset_clicked();
  void on_pushButtonDrawPlot_clicked();
  void on_pushButtonCalculate_clicked();
  void on_pushButtonDrawPlot_a_clicked();
  void on_pushButtonCalculate_a_clicked();

 private:
  auto drawGraph(QCustomPlot* plot) -> void;
  auto setScale(QCustomPlot* plot, const QVector<double>& dates,
                const QVector<double>& values, int days = 0) -> void;
  auto drawNewton() -> void;
  auto drawSpline() -> void;
  auto calculateNewton(size_t degree, double value) -> double;
  auto calculateSpline(double value) -> double;
  auto calculateApprox(int degree, double value) -> double;

  Ui::MainWindow* ui;
  s21::Model* model_instance_;
  size_t days_ext_{0};
  const QPen kGraphColors[6]{QColor(Qt::darkGray),    QColor(Qt::red),
                             QColor(Qt::blue),        QColor(Qt::darkGreen),
                             QColor(Qt::darkMagenta), QColor(Qt::darkCyan)};
};

#endif  //  SRC_MAINWINDOW_H_
