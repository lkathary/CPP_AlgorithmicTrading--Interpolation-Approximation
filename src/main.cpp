#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  setlocale(LC_ALL, "en_US.UTF-8");
  MainWindow w;
  w.show();
  return a.exec();
}
