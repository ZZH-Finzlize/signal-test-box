#include "mainWindow.h"
#include <cmath>
#include <QMessageBox>
#include <QDebug>
#include "grammar.tab.hpp"
#include "symTable.h"
#include "innerFun.h"
using namespace QtCharts;
//                                    HZ  KHZ    MHZ
const float MainWindow::fsScale[3] = { 1, 1000, 1000000 };

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), sigSuffix(0)
{
    ui.setupUi(this);
    // ui.pSignalChart->setMinimumSize(150, 150);

    auto pChart = new QChart();
    pChart->setAnimationOptions(QChart::AllAnimations);
    pChart->setBackgroundVisible();
    pChart->autoFillBackground();
    ui.pSignalChart->setChart(pChart);
    ui.pSignalList->addActions({ ui.actNewSig, ui.actDelSig });
    this->calNum = ui.pCalNum->text().toInt();

    connect(ui.actNewSig, &QAction::triggered, this, &MainWindow::addSignal);
    connect(ui.actDelSig, &QAction::triggered, this, &MainWindow::delSignal);

    connect(ui.pSignalList, &QListWidget::currentItemChanged, this, &MainWindow::enableExpress);

    connect(ui.pCalculateButton, &QPushButton::clicked, this, &MainWindow::calculateCurSig);
    connect(ui.pCalNum, &QLineEdit::editingFinished, this, [this]() {
        //这里要屏蔽掉输入框的信号,因为下面的MessageBox会获取输入焦点,所以会再次触发editingFinished
        ui.pCalNum->blockSignals(true);
        
        int newValue = ui.pCalNum->text().toInt();
        if (newValue > 4096 || newValue <= 0)
        {
            QMessageBox::critical(this, tr("计算点数错误"), tr("%1 是非法值").arg(newValue));
            newValue = this->calNum;
        }
        else
        {
            this->calNum = newValue;
        }

        //这里究竟是保持当前编辑值还是恢复上一次的正确值,两种策略好像都合理,暂定保持编辑值
        // ui.pCalNum->setText(QString("%1").arg(newValue));
        ui.pCalNum->blockSignals(false);
    });
}

MainWindow::~MainWindow()
{
}

void MainWindow::addSignal(void)
{
    QString itemName = this->sigName.arg(this->sigSuffix++);
    auto newItem = new QListWidgetItem(itemName);
    newItem->setFlags(newItem->flags() | Qt::ItemIsEditable);
    ui.pSignalList->addItem(newItem);
    SigSymTable.insert(itemName, newItem);
}

void MainWindow::delSignal(void)
{
    auto itemToDel =  ui.pSignalList->takeItem(ui.pSignalList->currentRow());
    if (ui.pSignalList->currentRow() == -1)//若删除后当前选择项非法,则重新进入禁止编辑表达式的状态
    {
        connect(ui.pSignalList, &QListWidget::currentItemChanged, this, &MainWindow::enableExpress);
        ui.pSignalExpress->setDisabled(true);
    }

    SigSymTable.remove(itemToDel->text());
    delete itemToDel;
}

void MainWindow::enableExpress(void)
{
    ui.pSignalExpress->setEnabled(true);
    disconnect(ui.pSignalList, &QListWidget::currentItemChanged, this, &MainWindow::enableExpress);
}

void MainWindow::on_pSignalList_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous)
{
    //保存上一个
    if (nullptr != previous)
    {
        previous->setData(this->signalExpressRole, ui.pSignalExpress->toPlainText());
    }

    //载入下一个
    if (nullptr != current)
    {
        const QString& expr = current->data(this->signalExpressRole).toString();
        ui.pSignalExpress->setPlainText(expr);
        if (not expr.isEmpty())
            this->calculateCurSig();
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
    textToParse = ui.pSignalExpress->toPlainText();
    fs = ui.pFs->text().toFloat() * this->fsScale[ui.pFsScale->currentIndex()];
    resetParser();
    yyparse();

    if (yyerrorCount == 0 || nullptr != root)
    {
        // QVector<float> dataArray(this->calNum);
        // QList<float> dataArray1;
        // dataArray1.fromVector(dataArray);

        if (root->compile())
        {
            resetInnerFun();
            ui.pSignalChart->chart()->removeAllSeries();
            auto series = new QSplineSeries();
            for (calPoint = 0;calPoint < this->calNum;calPoint++)
            {
                series->append(calPoint, root->calculate());
            }
            series->setName(ui.pSignalList->currentItem()->text());
            ui.pSignalChart->chart()->addSeries(series);
        }
    }

    if (nullptr != root)
    {
        delete root;
        root = nullptr;
    }
}
