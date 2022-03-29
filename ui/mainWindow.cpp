#include "mainWindow.h"
#include <cmath>
#include <QMessageBox>
#include <QDebug>
#include "grammar.tab.hpp"
using namespace QtCharts;

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), sigSuffix(0)
{
    ui.setupUi(this);
    // ui.pSignalChart->setMinimumSize(150, 150);

    auto pChart = new QChart();
    pChart->setAnimationOptions(QChart::AllAnimations);
    ui.pSignalChart->setChart(pChart);
    ui.pSignalList->addActions({ ui.actNewSig, ui.actDelSig });

    connect(ui.actNewSig, &QAction::triggered, this, &MainWindow::addSignal);
    connect(ui.actDelSig, &QAction::triggered, this, &MainWindow::delSignal);

    connect(ui.pSignalList, &QListWidget::currentItemChanged, this, &MainWindow::enableExpress);

    connect(ui.pCalculateButton, &QPushButton::clicked, this, &MainWindow::calculateCurSig);
}

MainWindow::~MainWindow()
{
}

void MainWindow::addSignal(void)
{
    auto newItem = new QListWidgetItem(this->sigName.arg(this->sigSuffix++));
    newItem->setFlags(newItem->flags() | Qt::ItemIsEditable);
    ui.pSignalList->addItem(newItem);
}

void MainWindow::delSignal(void)
{
    delete ui.pSignalList->takeItem(ui.pSignalList->currentRow());
    if (ui.pSignalList->currentRow() == -1)//若删除后当前选择项非法,则重新进入禁止编辑表达式的状态
    {
        connect(ui.pSignalList, &QListWidget::currentItemChanged, this, &MainWindow::enableExpress);
        ui.pSignalExpress->setDisabled(true);
    }
}

void MainWindow::enableExpress(void)
{
    ui.pSignalExpress->setEnabled(true);
    disconnect(ui.pSignalList, &QListWidget::currentItemChanged, this, &MainWindow::enableExpress);
}

void MainWindow::on_pSignalList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    //保存上一个
    if(nullptr != previous)
    {
        previous->setData(this->signalExpressRole, ui.pSignalExpress->toPlainText());
    }

    //载入下一个
    if (nullptr != current)
    {
        ui.pSignalExpress->setPlainText(current->data(this->signalExpressRole).toString());
    }
    else
    {
        ui.pSignalExpress->clear();
    }
}

void MainWindow::signalExpEditDone(void)
{
    auto curItem = ui.pSignalList->currentItem();
    if (nullptr != curItem)
    {
        curItem->setData(this->signalExpressRole, ui.pSignalExpress->toPlainText());
    }
}

void MainWindow::calculateCurSig(void)
{
    yylex();
    ui.pSignalChart->chart()->removeAllSeries();
    auto series = new QSplineSeries();
    for (calPoint = 0;calPoint < 100;calPoint++)
    {   
        series->append(calPoint, root->calculate());
    }
    ui.pSignalChart->chart()->addSeries(series);
}
