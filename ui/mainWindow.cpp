#include "mainWindow.h"
using namespace QtCharts;

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    ui.setupUi(this);
    this->pChart = new QChart();
    auto pChartView = new QChartView(this);
    pChartView->setChart(this->pChart);
    ui.verticalLayout_inner->addWidget(pChartView);
}

MainWindow::~MainWindow()
{
    delete this->pChart;
}
