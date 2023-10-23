#include "mainwindow.h"

#include <QFileDialog>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      model_instance_(new s21::Model) {
  ui->setupUi(this);
  this->setFixedSize(this->geometry().width(), this->geometry().height());

  ui->interPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));
  ui->approxPlot->setLocale(QLocale(QLocale::English, QLocale::UnitedKingdom));

  QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
  dateTicker->setDateTimeFormat("yyyy-MM-dd");
  dateTicker->setTickCount(16);
  ui->interPlot->xAxis->setTicker(dateTicker);
  ui->approxPlot->xAxis->setTicker(dateTicker);
  ui->interPlot->xAxis->setTickLabelRotation(-90);
  ui->approxPlot->xAxis->setTickLabelRotation(-90);

  ui->interPlot->legend->setBrush(QColor(215, 217, 168, 100));
  ui->approxPlot->legend->setBrush(QColor(215, 217, 168, 100));
  QFont legend_font = font();
  legend_font.setPointSize(8);
  ui->interPlot->legend->setFont(legend_font);
  ui->approxPlot->legend->setFont(legend_font);
  ui->interPlot->axisRect()->insetLayout()->setInsetAlignment(
      0, Qt::AlignTop | Qt::AlignLeft);
  ui->approxPlot->axisRect()->insetLayout()->setInsetAlignment(
      0, Qt::AlignTop | Qt::AlignLeft);

  ui->interPlot->setInteraction(QCP::iRangeZoom, true);
  ui->approxPlot->setInteraction(QCP::iRangeZoom, true);
  ui->interPlot->setInteraction(QCP::iRangeDrag, true);
  ui->approxPlot->setInteraction(QCP::iRangeDrag, true);

  s21::Controller& ctrl = s21::Controller::GetInstance();
  ctrl.Connect(model_instance_);
}

MainWindow::~MainWindow() {
  delete ui;
  delete model_instance_;
}

void MainWindow::on_pushButtonInfo_clicked() {
  s21::Controller& ctrl = s21::Controller::GetInstance();
  ui->textInfo->append("Value, Data (" +
                       QString::number(ctrl.GetData().size()) + " points):\n");
  char date[11];
  for (auto& it : ctrl.GetData()) {
    strftime(date, 11, "%Y-%m-%d", &it.first);
    ui->textInfo->append(QString::number(it.second) + "\t" +
                         QString::fromUtf8(date, 11));
  }
}

void MainWindow::on_pushButtonClear_clicked() { ui->textInfo->clear(); }

void MainWindow::on_pushButtonOpen_clicked() {
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), ".",
                                                  tr("csv files (*.csv)"));
  if (!fileName.isNull()) {
    s21::Controller& ctrl = s21::Controller::GetInstance();
    ctrl.Clear();
    std::string result = ctrl.LoadFromFile(fileName.toStdString());
    ui->textInfo->append(QString::fromStdString(result));
    ui->interPlot->clearGraphs();
    ui->approxPlot->clearGraphs();
    ui->interPlot->replot();
    ui->approxPlot->replot();
    if (!ctrl.GetData().empty()) {
      drawGraph(ui->interPlot);
      drawGraph(ui->approxPlot);
      ui->spinBoxNumPoints->setMinimum(ctrl.GetData().size());
      ui->spinBoxNumPoints_a->setMinimum(ctrl.GetData().size());
      ui->spinBoxNumPoints->setValue(ctrl.GetData().size() * 10);
      ui->spinBoxNumPoints_a->setValue(ctrl.GetData().size() * 10);
      ui->lineEditResultNewton->setText("");
      ui->lineEditResultSpline->setText("");
      ui->lineEditResultApprox->setText("");
      ui->spinBoxDaysExt->setValue(0);
      ui->dateTimeEdit->setDateTime(QDateTime::fromSecsSinceEpoch(
          std::mktime(&(ctrl.GetData().front().first))));
      ui->dateTimeEdit_a->setDateTime(QDateTime::fromSecsSinceEpoch(
          std::mktime(&(ctrl.GetData().front().first))));
    }
  }
}

void MainWindow::on_pushButtonCleanData_clicked() {
  s21::Controller& ctrl = s21::Controller::GetInstance();
  ctrl.Clear();
  ui->textInfo->append("All data cleared");
  ui->interPlot->clearGraphs();
  ui->approxPlot->clearGraphs();
  ui->interPlot->legend->setVisible(false);
  ui->approxPlot->legend->setVisible(false);
  ui->interPlot->replot();
  ui->approxPlot->replot();
}

void MainWindow::on_pushButtonReset_clicked() {
  if (!ui->tabWidget->currentIndex()) {
    ui->interPlot->clearGraphs();
    drawGraph(ui->interPlot);
  } else {
    ui->approxPlot->clearGraphs();
    drawGraph(ui->approxPlot);
  }
}

void MainWindow::on_pushButtonDrawPlot_clicked() {
  if (ui->radioButtonNewton->isChecked()) {
    drawNewton();
  } else {
    drawSpline();
  }
}

void MainWindow::on_pushButtonCalculate_clicked() {
  s21::Controller& ctrl = s21::Controller::GetInstance();
  std::vector<s21::DataPoint> graph = ctrl.GetData();

  if (!graph.empty()) {
    double value =
        static_cast<double>(ui->dateTimeEdit->dateTime().toSecsSinceEpoch());
    if ((std::mktime(&(graph.front().first)) - s21::kEps) > value ||
        (std::mktime(&(graph.back().first)) + s21::kEps) < value) {
      ui->lineEditResultNewton->setText("n/a");
      ui->lineEditResultSpline->setText("n/a");
      ui->textInfo->append("Cannot be calculated, date out of range");
      return;
    }
    size_t degree = static_cast<size_t>(ui->spinBoxDegreePoly->value());
    if (graph.size() > degree) {
      double result = calculateNewton(degree, value);
      ui->lineEditResultNewton->setText(QString::number(result));
      ui->textInfo->append("Newton result: " + QString::number(result));
    } else {
      ui->lineEditResultNewton->setText("n/a");
      ui->textInfo->append("Cannot be calculated Newton, not enough data");
    }
    if (graph.size() > 2) {
      double result = calculateSpline(value);
      ui->lineEditResultSpline->setText(QString::number(result));
      ui->textInfo->append("Spline result: " + QString::number(result));
    } else {
      ui->lineEditResultSpline->setText("n/a");
      ui->textInfo->append("Cannot be calculated Spline, not enough data");
    }
  } else {
    ui->lineEditResultNewton->setText("n/a");
    ui->lineEditResultSpline->setText("n/a");
    ui->textInfo->append("Cannot be calculated, empty data");
  }
}

void MainWindow::on_pushButtonDrawPlot_a_clicked() {
  s21::Controller& ctrl = s21::Controller::GetInstance();
  std::vector<s21::DataPoint> graph = ctrl.GetData();

  if (!graph.empty()) {
    size_t intervals = static_cast<size_t>(ui->spinBoxNumPoints_a->value()) - 1;
    size_t degree = static_cast<size_t>(ui->spinBoxDegreePoly_a->value());
    if (graph.size() > 1) {
      if (days_ext_ != static_cast<size_t>(ui->spinBoxDaysExt->value())) {
        days_ext_ = static_cast<size_t>(ui->spinBoxDaysExt->value());
        ui->approxPlot->clearGraphs();
        drawGraph(ui->approxPlot);
      }
      double begin = std::mktime(&(graph.front().first));
      double end =
          std::mktime(&(graph.back().first)) + days_ext_ * s21::kSecInDay;
      double step = (end - begin) / intervals;

      QVector<double> values, dates;
      ctrl.initApproximation(graph, degree);
      for (double current = begin; current < end + s21::kEps; current += step) {
        dates.push_back(current);
        values.push_back(ctrl.GetApproxValue(current));
      }

      int count = ui->approxPlot->graphCount();
      if (count <= s21::kMaxCountGraph) {
        ui->approxPlot->addGraph();
        ui->approxPlot->graph(count)->setData(dates, values);
        ui->approxPlot->graph()->setPen(kGraphColors[count]);
        ui->approxPlot->graph()->setName(
            "Approx, " + QString::number(intervals + 1) + " points, " +
            QString::number(degree) + " degree");
        ui->approxPlot->replot();
        ui->textInfo->append("Approximation is plotted");
      } else {
        ui->textInfo->append(
            "Cannot be plotted, maximum number of graphs reached");
      }
    } else {
      ui->textInfo->append("Cannot be plotted, not enough data");
    }
  } else {
    ui->textInfo->append("Cannot be plotted, empty data");
  }
}

void MainWindow::on_pushButtonCalculate_a_clicked() {
  s21::Controller& ctrl = s21::Controller::GetInstance();
  std::vector<s21::DataPoint> graph = ctrl.GetData();

  if (!graph.empty()) {
    double value =
        static_cast<double>(ui->dateTimeEdit_a->dateTime().toSecsSinceEpoch());
    if ((std::mktime(&(graph.front().first)) - s21::kEps) > value) {
      ui->lineEditResultApprox->setText("n/a");
      ui->textInfo->append("Cannot be calculated, date out of range");
      return;
    }
    if (graph.size() > 1) {
      size_t degree = static_cast<size_t>(ui->spinBoxDegreePoly_a->value());
      double result = calculateApprox(degree, value);
      ui->lineEditResultApprox->setText(QString::number(result));
      ui->textInfo->append("Approximation result: " + QString::number(result));
    } else {
      ui->lineEditResultApprox->setText("n/a");
      ui->textInfo->append(
          "Cannot be calculated Approximation, not enough data");
    }
  } else {
    ui->lineEditResultApprox->setText("n/a");
    ui->textInfo->append("Cannot be calculated, empty data");
  }
}

void MainWindow::drawGraph(QCustomPlot* plot) {
  s21::Controller& ctrl = s21::Controller::GetInstance();
  std::vector<s21::DataPoint> graph = ctrl.GetData();

  if (!graph.empty()) {
    QVector<double> values, dates;
    for (auto& it : graph) {
      values.push_back(it.second);
      dates.push_back(std::mktime(&(it.first)));
    }

    plot->addGraph();
    plot->graph(0)->setData(dates, values);

    plot->graph(0)->setPen(kGraphColors[0]);
    plot->graph(0)->setLineStyle(QCPGraph::lsNone);
    plot->graph(0)->setScatterStyle(
        QCPScatterStyle(QCPScatterStyle::ssDisc, 4));

    plot->graph(0)->setName("Input data");
    plot->legend->setVisible(true);

    setScale(plot, dates, values);
    plot->replot();
  }
}

void MainWindow::setScale(QCustomPlot* plot, const QVector<double>& dates,
                          const QVector<double>& values, int days) {
  double left{dates.front()}, right{dates.back() + days * s21::kSecInDay};
  double max{values.front()}, min{values.front()};
  for (auto& it : values) {
    if (it > max) max = it;
    if (it < min) min = it;
  }
  plot->xAxis->setRange(left - s21::kPadCoeff * (right - left),
                        right + s21::kPadCoeff * (right - left));
  plot->yAxis->setRange(min - s21::kPadCoeff * (max - min),
                        max + s21::kPadCoeff * (max - min));
}

void MainWindow::drawNewton() {
  s21::Controller& ctrl = s21::Controller::GetInstance();
  std::vector<s21::DataPoint> graph = ctrl.GetData();

  if (!graph.empty()) {
    size_t intervals = static_cast<size_t>(ui->spinBoxNumPoints->value()) - 1;
    size_t degree = static_cast<size_t>(ui->spinBoxDegreePoly->value());
    if (graph.size() > degree) {
      double begin = std::mktime(&(graph.front().first));
      double end = std::mktime(&(graph.back().first));
      double step = (end - begin) / intervals;
      double current = begin;

      QVector<double> values, dates;
      std::vector<s21::DataPoint> segment;
      for (size_t i = 0; i < graph.size() - 1; i += degree) {
        segment.clear();
        for (size_t j = 0; j < (degree + 1); ++j) {
          if ((i + degree) < graph.size()) {
            segment.push_back(graph[i + j]);
          } else {
            for (size_t k = graph.size() - degree - 1; k < graph.size(); ++k) {
              segment.push_back(graph[k]);
            }
            break;
          }
        }
        ctrl.initNewtonPolynomial(segment);
        for (; current < std::mktime(&(segment.back().first)) + s21::kEps;) {
          dates.push_back(current);
          values.push_back(ctrl.GetNewtonValue(current));
          current += step;
        }
      }
      int count = ui->interPlot->graphCount();
      if (count <= s21::kMaxCountGraph) {
        ui->interPlot->addGraph();
        ui->interPlot->graph(count)->setData(dates, values);
        ui->interPlot->graph()->setPen(kGraphColors[count]);
        ui->interPlot->graph()->setName(
            "Newton, " + QString::number(intervals + 1) + " points, " +
            QString::number(degree) + " degree");
        ui->interPlot->replot();
        ui->textInfo->append("Newton polynomial is plotted");
      } else {
        ui->textInfo->append(
            "Cannot be plotted, maximum number of graphs reached");
      }
    } else {
      ui->textInfo->append("Cannot be plotted, not enough data");
    }
  } else {
    ui->textInfo->append("Cannot be plotted, empty data");
  }
}

void MainWindow::drawSpline() {
  s21::Controller& ctrl = s21::Controller::GetInstance();
  std::vector<s21::DataPoint> graph = ctrl.GetData();

  if (!graph.empty()) {
    size_t intervals = static_cast<size_t>(ui->spinBoxNumPoints->value()) - 1;
    if (graph.size() > 2) {
      double begin = std::mktime(&(graph.front().first));
      double end = std::mktime(&(graph.back().first));
      double step = (end - begin) / intervals;

      QVector<double> values, dates;
      ctrl.initCubicSpline(graph);
      for (double current = begin; current < end + s21::kEps; current += step) {
        dates.push_back(current);
        values.push_back(ctrl.GetSplineValue(current));
      }

      int count = ui->interPlot->graphCount();
      if (count <= s21::kMaxCountGraph) {
        ui->interPlot->addGraph();
        ui->interPlot->graph(count)->setData(dates, values);
        ui->interPlot->graph()->setPen(kGraphColors[count]);
        ui->interPlot->graph()->setName(
            "Cubic Spline, " + QString::number(intervals + 1) + " points");
        ui->interPlot->replot();
        ui->textInfo->append("Cubic Spline is plotted");
      } else {
        ui->textInfo->append(
            "Cannot be plotted, maximum number of graphs reached");
      }
    } else {
      ui->textInfo->append("Cannot be plotted, not enough data");
    }
  } else {
    ui->textInfo->append("Cannot be plotted, empty data");
  }
}

double MainWindow::calculateNewton(size_t degree, double value) {
  s21::Controller& ctrl = s21::Controller::GetInstance();
  std::vector<s21::DataPoint> graph = ctrl.GetData();

  std::vector<s21::DataPoint> segment;
  for (size_t i = 0; i < graph.size() - 1; i += degree) {
    if ((i + degree) < graph.size()) {
      if ((std::mktime(&(graph[i].first)) - s21::kEps) < value &&
          (std::mktime(&(graph[i + degree].first)) + s21::kEps) > value) {
        for (size_t j = 0; j < (degree + 1); ++j) {
          segment.push_back(graph[i + j]);
        }
        break;
      }
    } else {
      for (size_t k = graph.size() - degree - 1; k < graph.size(); ++k) {
        segment.push_back(graph[k]);
      }
    }
  }
  ctrl.initNewtonPolynomial(segment);
  return ctrl.GetNewtonValue(value);
}

double MainWindow::calculateSpline(double value) {
  s21::Controller& ctrl = s21::Controller::GetInstance();
  std::vector<s21::DataPoint> graph = ctrl.GetData();

  ctrl.initCubicSpline(graph);
  return ctrl.GetSplineValue(value);
}

double MainWindow::calculateApprox(int degree, double value) {
  s21::Controller& ctrl = s21::Controller::GetInstance();
  std::vector<s21::DataPoint> graph = ctrl.GetData();

  ctrl.initApproximation(graph, degree);
  return ctrl.GetApproxValue(value);
}
