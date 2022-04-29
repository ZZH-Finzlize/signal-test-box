#include "ChartView.h"
#include <qDebug>

using namespace QtCharts;

void ChartView::setUp()
{
    this->pPosTip = new QLabel(this);
    this->pPosTip->setMinimumSize(QSize(60, 30));
    this->pPosTip->show();
    this->prePos.setX(0);
    this->prePos.setY(0);
    this->isPress = false;
}

void ChartView::mouseMoveEvent(QMouseEvent* event)
{
    auto pChart = this->chart();

    if (nullptr == pChart)
        return;//直接返回没有调用父类的事件函数,不知道会不会有影响

    auto curPos = event->pos();

    if (this->isPress)
    {
        int deltaX = this->prePos.x() - curPos.x();
        int deltaY = curPos.y() - this->prePos.y();
        
        pChart->scroll(deltaX, deltaY);
    }

    auto pos = pChart->mapToValue(curPos);
    auto axes = pChart->axes();
    QValueAxis* axisX = dynamic_cast<QValueAxis*>(axes[0]);
    QValueAxis* axisY = dynamic_cast<QValueAxis*>(axes[1]);
    auto pSerise = dynamic_cast<QLineSeries*>(pChart->series()[0]);

    float xMax = axisX->max();
    float x = pos.x();

    if (x < 0)
        x = 0;
    else if (x > xMax)
        x = xMax;

    if (pSerise->count() > (int) x)
    {
        float y = pSerise->at((int) x).y();

        this->pPosTip->setText(QString("X: %1\nY: %2").arg(x).arg(y));
    }

    this->prePos = curPos;
    QGraphicsView::mouseMoveEvent(event);
}

void ChartView::mousePressEvent(QMouseEvent* event)
{
    if (Qt::LeftButton == event->button())
    {
        this->isPress = true;
    }
}

void ChartView::mouseReleaseEvent(QMouseEvent* event)
{
    if (Qt::LeftButton == event->button())
    {
        this->isPress = false;
    }
}

void ChartView::wheelEvent(QWheelEvent* event)
{

}
