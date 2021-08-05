#ifndef MYCHART_H
#define MYCHART_H

#include <QtCharts/QChart>
#include <QtCore/QTimer>
#include <settings.h>
#include <vector>
QT_CHARTS_BEGIN_NAMESPACE
class QSplineSeries;
class QValueAxis;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class Mychart : public QChart
{
public:
    Mychart(QGraphicsItem *parent = 0, Qt::WindowFlags wFlags = 0,int type=0,int linenum=4,int companytypecode=0);
    ~Mychart();
    void changeYScale(double y_bt=-10000000,double y_tp=-10000000);
public slots:
    void getMessage(int charttype,int mx,int index,double tcf,double tcs=-1,double tct=-1);
    void show_hidden_Series(int index,bool selected);
    void changeSplineName(int companytypecode);
private:
    int chartType;
    QSplineSeries * my_series[4]; 
    QValueAxis *axisX;
    QValueAxis *axisY;
    int companytypecode;
    int linenums;
//    QVector<QList<QPointF> *> points;
    std::vector<int> pointsNum;
};

#endif // MYCHART_H
