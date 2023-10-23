QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Approximation/approximation.cpp \
    Approximation/gauss.cpp \
    NewtonInterpolation/newton_interpolation.cpp \
    SplineInterpolation/spline_interpolation.cpp \
    main.cpp \
    mainwindow.cpp \
    model.cpp \
    qcustomplot.cpp

HEADERS += \
    Approximation/approximation.h \
    Approximation/gauss.h \
    NewtonInterpolation/newton_interpolation.h \
    SplineInterpolation/spline_interpolation.h \
    controller.h \
    mainwindow.h \
    model.h \
    qcustomplot.h \
    types.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
