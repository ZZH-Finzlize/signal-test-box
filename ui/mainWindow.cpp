#include "mainWindow.h"
#include <cmath>
#include <QMessageBox>
#include "log.h"
#include "symTable.h"
#include "innerFun.h"
#include "ChartView.h"
#include "SignalItem.h"
#include "compiler.h"
#include "calculator.h"

using namespace QtCharts;
//                                    HZ  KHZ    MHZ
const float MainWindow::fsScale[3] = { 1, 1000, 1000000 };
//此处的正则和词法分析器对symbol的要求一致
const QRegExp MainWindow::sigNameRule("[a-zA-Z_][a-zA-Z0-9_]*");

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), sigSuffix(0)
{
    UI_INFO("Start init");
    ui.setupUi(this);
    this->pSeries = new QLineSeries;
    this->pAxisX = new QValueAxis();
    this->pAxisY = new QValueAxis();

    auto pChart = new QChart();
    pChart->setAnimationOptions(QChart::NoAnimation);
    pChart->setBackgroundVisible();
    pChart->autoFillBackground();
    pChart->addAxis(this->pAxisX, Qt::AlignBottom);
    pChart->addAxis(this->pAxisY, Qt::AlignLeft);
    pChart->legend()->setVisible(false);
    pChart->addSeries(this->pSeries);
    this->pSeries->attachAxis(this->pAxisX);
    this->pSeries->attachAxis(this->pAxisY);

    ui.pSignalChart->setChart(pChart);
    ui.pSignalList->addActions({ ui.actNewSig, ui.actDelSig });

    Calculator_t::getInst().setTotolPoint(ui.pCalNum->text().toInt());

    ui.pCalNum->setValidator(new QRegExpValidator(QRegExp("\\d+")));

    connect(ui.actNewSig, &QAction::triggered, this, &MainWindow::addSignal);
    connect(ui.actDelSig, &QAction::triggered, this, &MainWindow::delSignal);

    connect(ui.pSignalList, &QListWidget::currentItemChanged, this, &MainWindow::enableExpress);

    connect(ui.pCalculateButton, &QPushButton::clicked, this, &MainWindow::calculateCurSig);
    connect(ui.pSignalList, &QListWidget::itemChanged, this, &MainWindow::itemChanged);
    connect(ui.pCalNum, &QLineEdit::editingFinished, this, [this]() {
        //这里要屏蔽掉输入框的信号,因为下面的MessageBox会获取输入焦点,所以会再次触发editingFinished
        ui.pCalNum->blockSignals(true);

        int newValue = ui.pCalNum->text().toInt();
        auto& calculator = Calculator_t::getInst();

        if (newValue > 1024 || newValue <= 0)
        {
            QMessageBox::critical(this, tr("计算点数错误"), tr("%1 是非法值").arg(newValue));
            newValue = calculator.getTotolPoint();
        }
        else
        {
            calculator.setTotolPoint(newValue);
        }

        //这里究竟是保持当前编辑值还是恢复上一次的正确值,两种策略好像都合理,暂定保持编辑值
        // ui.pCalNum->setText(QString("%1").arg(newValue));
        ui.pCalNum->blockSignals(false);
    });

    Compiler_t::loadExtLibs("./lib");
    UI_INFO("Init done");
}

MainWindow::~MainWindow()
{
    UI_INFO("Destroy");
}

void MainWindow::addSignal(void)
{
    QString itemName = this->sigName.arg(this->sigSuffix++);
    auto newItem = new SignalItem(itemName);
    newItem->setFlags(newItem->flags() | Qt::ItemIsEditable);
    ui.pSignalList->addItem(newItem);

    if (SigSymTable.insert(itemName, newItem))
    {
        this->curItemText = itemName;
        UI_INFO("Add signal: %s", itemName.toStdString().c_str());
    }
    else
    {
        QMessageBox::critical(this, tr("名称重复"), tr("此名称与现有另一信号重名"));
    }

    ui.pSignalList->editItem(newItem);

}

void MainWindow::delSignal(void)
{
    auto itemToDel = ui.pSignalList->takeItem(ui.pSignalList->currentRow());
    if (ui.pSignalList->currentRow() == -1)//若删除后当前选择项非法,则重新进入禁止编辑表达式的状态
    {
        connect(ui.pSignalList, &QListWidget::currentItemChanged, this, &MainWindow::enableExpress);
        ui.pSignalExpress->setDisabled(true);
    }

    if (itemToDel)
    {
        SigSymTable.remove(itemToDel->text());
        delete itemToDel;
    }
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
        // this->saveExpToItem(previous, ui.pSignalExpress->toPlainText());
        auto pItem = static_cast<SignalItem*>(previous);
        pItem->setSourceCode(ui.pSignalExpress->toPlainText());
    }

    //载入下一个
    if (nullptr != current)
    {
        const QString& expr = static_cast<SignalItem*>(current)->getSourceCode();
        ui.pSignalExpress->setPlainText(expr);
        if (not expr.isEmpty())
            this->calculateCurSig();
        else
            this->pSeries->clear();
        this->curItemText = current->text();
    }
    else
    {
        UI_INFO("Invaild selection clear curItemText");
        this->curItemText.clear();
        ui.pSignalExpress->clear();
        this->pSeries->clear();
    }
}

void MainWindow::itemChanged(QListWidgetItem* item)
{
    if (not this->curItemText.isEmpty())
    {
        const QString& newName = item->text();
        if (this->sigNameRule.exactMatch(newName))//新的信号名称符合要求
        {
            UI_INFO("Item changed from: %s to: %s", this->curItemText.toStdString().c_str(), newName.toStdString().c_str());
            SigSymTable.remove(this->curItemText);
            this->curItemText = newName;
            if (SigSymTable.insert(this->curItemText, static_cast<SignalItem*>(item)))
            {
                return;
            }
            else
            {
                QMessageBox::critical(this, tr("名称重复"), tr("此名称与现有另一信号重名"));
            }
        }
        else
        {
            QMessageBox::critical(this, tr("名称非法"), tr("此名称不符合信号命名规则"));
        }

        ui.pSignalList->blockSignals(true);
        item->setText(this->curItemText);
        ui.pSignalList->blockSignals(false);
    }
}

void MainWindow::calculateCurSig(void)
{
    auto pItem = static_cast<SignalItem*>(ui.pSignalList->currentItem());
    pItem->setSourceCode(ui.pSignalExpress->toPlainText());//将编辑框代码保存至item

    //获取编译器和计算器的实例
    auto& calculator = Calculator_t::getInst();
    auto& compiler = Compiler_t::getInst();

    //设置计算器使用的采样率
    float fs = ui.pFs->text().toFloat() * this->fsScale[ui.pFsScale->currentIndex()];
    calculator.setFS(fs);

    //从计算器获取总计算点数
    int calNum = calculator.getTotolPoint();

    float maxValue = 0, minValue = 0;

    if (true == compiler.compile(pItem))//计算之前先编译当前item,item内部会根据item的源码是否修改选择性编译
    {
        float* res = new float[calNum];
        calculator.calculate(pItem, res);

        pSeries->clear();

        for (int calPoint = 0;calPoint < calNum;calPoint++)
        {
            float curValue = res[calPoint];
            maxValue = __max(curValue, maxValue);
            minValue = __min(curValue, minValue);
            this->pSeries->append(fftIsCalled ? calPoint * fs / calNum : calPoint, curValue);
        }
        delete[] res;

        this->pSeries->setName(ui.pSignalList->currentItem()->text());

        if (true == fftIsCalled)//频谱模式,横轴需要直接解算为对应的频率值
        {
            this->pAxisX->setRange(0, fs);
        }
        else//非频谱模式, 范围直接就是值的范围
        {
            this->pAxisX->setRange(0, calNum);
        }

        this->pAxisY->setRange(minValue * 1.15 - 1, maxValue * 1.15 + 1);
    }
    else
    {
        UI_ERROR("Signal compile fail");
    }

}
