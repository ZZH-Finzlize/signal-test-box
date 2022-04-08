#include "ChartView.h"
#include <qDebug>

using namespace QtCharts;

void ChartView::setUp()
{
    this->pPosTip = new QLabel(this);
    this->pPosTip->setMinimumSize(QSize(60, 30));
    this->pPosTip->show();
}

void ChartView::mouseMoveEvent(QMouseEvent* event)
{
    auto pChart = this->chart();
    auto pos = pChart->mapToValue(event->pos());
    auto axes = pChart->axes();
    QValueAxis* axisX = dynamic_cast<QValueAxis*>(axes[0]);
    QValueAxis* axisY = dynamic_cast<QValueAxis*>(axes[1]);

    float xMin = axisX->min();
    float xMax = axisX->max();
    float yMin = axisY->min();
    float yMax = axisY->max();

    float x = pos.x();
    float y = pos.y();

    if (x < xMin)
        x = xMin;
    else if (x > xMax)
        x = xMax;

    if (y < yMin)
        y = yMin;
    else if (y > yMax)
        y = yMax;

    this->pPosTip->setText(QString("X: %1\nY: %2").arg(x).arg(y));

    QGraphicsView::mouseMoveEvent(event);
}
