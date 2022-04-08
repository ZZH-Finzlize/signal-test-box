#pragma once
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLineSeries>
#include <QLabel>

class ChartView :public QtCharts::QChartView
{
private:
    void setUp();
protected:
    QLabel *pPosTip;
public:
    explicit ChartView(QWidget* parent = nullptr) : QChartView(parent) { this->setUp(); }
    explicit ChartView(QtCharts::QChart* chart, QWidget* parent = nullptr): QChartView(chart, parent) { this->setUp(); }
    ~ChartView() {}
    

    virtual void mouseMoveEvent(QMouseEvent* event) override;
};
