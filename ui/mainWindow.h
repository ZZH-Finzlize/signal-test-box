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
#include <QtCharts/QLineSeries>
#include <QListWidgetItem>

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    unsigned int sigSuffix;
    const QString sigName = "sig%1";
    QString curItemText;
    static const QRegExp sigNameRule;
    QtCharts::QLineSeries* pSeries;
    QtCharts::QValueAxis* pAxisX, * pAxisY;
protected:
    Ui::MainWindow ui;

    int calNum;

    inline void saveExpToItem(QListWidgetItem* item, const QString& text)
    {
        ui.pSignalList->blockSignals(true);
        item->setData(this->signalExpressRole, text);
        ui.pSignalList->blockSignals(false);
    }

public:
    static const int signalExpressRole = Qt::UserRole + 2;
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    static const float fsScale[3];

    void addSignal(void);
    void delSignal(void);

    void enableExpress(void);

    void calculateCurSig(void);

    void itemChanged(QListWidgetItem* item);

public slots:
    void on_pSignalList_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);
};
