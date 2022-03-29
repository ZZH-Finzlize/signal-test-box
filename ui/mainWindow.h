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
#include <QtCharts/QValueAxis>
#include <QtCharts/QSplineSeries>
#include <QListWidgetItem>

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    unsigned int sigSuffix;
    const QString sigName = "signal%1";
protected:
    Ui::MainWindow ui;
    static const int signalExpressRole = Qt::UserRole + 2;
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void addSignal(void);
    void delSignal(void);

    void enableExpress(void);

    void signalExpEditDone(void);

    void calculateCurSig(void);

public slots:
    void on_pSignalList_currentItemChanged(QListWidgetItem* current, QListWidgetItem *previous);
};
    