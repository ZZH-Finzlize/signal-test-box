#include "mainWindow.h"
#include <cmath>
#include <QMessageBox>
#include <QDebug>
#include "symTable.h"
#include "innerFun.h"
#include "compile_common.h"

using namespace QtCharts;
//                                    HZ  KHZ    MHZ
const float MainWindow::fsScale[3] = { 1, 1000, 1000000 };
//此处的正则和词法分析器对symbol的要求一致
const QRegExp MainWindow::sigNameRule("[a-zA-Z_][a-zA-Z0-9_]*");

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), sigSuffix(0)
{
    ui.setupUi(this);
    // ui.pSignalChart->setMinimumSize(150, 150);

    auto pChart = new QChart();
    pChart->setAnimationOptions(QChart::AllAnimations);
    pChart->setBackgroundVisible();
    pChart->autoFillBackground();
    pChart->addAxis(new QValueAxis(), Qt::AlignBottom);
    pChart->addAxis(new QValueAxis(), Qt::AlignLeft);
    ui.pSignalChart->setChart(pChart);
    ui.pSignalList->addActions({ ui.actNewSig, ui.actDelSig });
    this->calNum = ui.pCalNum->text().toInt();

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

    if (SigSymTable.insert(itemName, newItem))
    {
        this->curItemText = itemName;
        qDebug() << "add new sig " << itemName << "to symTable and update curItemText";
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
        this->saveExpToItem(previous, ui.pSignalExpress->toPlainText());
    }

    //载入下一个
    if (nullptr != current)
    {
        const QString& expr = current->data(this->signalExpressRole).toString();
        ui.pSignalExpress->setPlainText(expr);
        if (not expr.isEmpty())
            this->calculateCurSig();
        else
            ui.pSignalChart->chart()->removeAllSeries();
        this->curItemText = current->text();
    }
    else
    {
        qDebug() << "invaild selection clear curItemText";
        this->curItemText.clear();
        ui.pSignalExpress->clear();
        ui.pSignalChart->chart()->removeAllSeries();
    }
}

void MainWindow::itemChanged(QListWidgetItem* item)
{
    if (not this->curItemText.isEmpty())
    {
        const QString& newName = item->text();
        if (this->sigNameRule.exactMatch(newName))//新的信号名称符合要求
        {
            qDebug() << "item changed from:" << this->curItemText << " to: " << newName;
            SigSymTable.remove(this->curItemText);
            this->curItemText = newName;
            if (SigSymTable.insert(this->curItemText, item))
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
    textToParse = ui.pSignalExpress->toPlainText();
    this->saveExpToItem(ui.pSignalList->currentItem(), textToParse);
    fs = ui.pFs->text().toFloat() * this->fsScale[ui.pFsScale->currentIndex()];
    resetParser();
    yyparse();

    if (yyerrorCount == 0 and nullptr != root)
    {
        root->resetRecCounter();
        if (root->compile())
        {
            float maxValue, minValue;
            allCalNum = this->calNum;
            resetInnerFun();
            auto pChart = ui.pSignalChart->chart();
            pChart->removeAllSeries();
            
            root->preCalculateT();
            float *res = new float[this->calNum];
            root->calculate(res);
            root->cleanPreCalT();

            auto series = new QLineSeries();
            for (int calPoint = 0;calPoint < this->calNum;calPoint++)
            {
                float curValue = res[calPoint];
                maxValue = __max(curValue, maxValue);
                minValue = __min(curValue, minValue);
                series->append(calPoint, curValue);
            }
            
            delete[] res;
            
            series->setName(ui.pSignalList->currentItem()->text());

            auto axisX = pChart->axes(Qt::Horizontal)[0];
            auto axisY = pChart->axes(Qt::Vertical)[0];
            axisX->setRange(0, this->calNum);
            axisY->setRange(minValue * 1.15 - 1, maxValue * 1.15 + 1);

            pChart->addSeries(series);
            series->attachAxis(axisX);
            series->attachAxis(axisY);
        }
    }

    if (nullptr != root)
    {
        delete root;
        root = nullptr;
    }
}
