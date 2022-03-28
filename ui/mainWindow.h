#pragma once
#include "ui_mainwindow.h"

#include <QString>
#include <QDebug>
#include <QMainWindow>
#include <QtCore>
#include <QMessageBox>
#include <QPushButton>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QtCharts::QChart* pChart;
protected:
    Ui::MainWindow ui;
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
};
