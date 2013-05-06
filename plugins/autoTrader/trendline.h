/*
 * (c) 2012 - Mattias Johansson
 *
 *
 *
 */

#include <QList>

class Trendline {

  public:
    Trendline(QList<int> yAxisValues, QList<int> xAxisValues);
    int slope;
    int intercept;
    int start;
    int end;

  private:
    void Initialize();
    int CalculateSlope();
    int CalculateIntercept();
    int CalculateStart();
    int CalculateEnd();

    QList<int> xAxisValues;
    QList<int> yAxisValues;
    int count;
    int xAxisValuesSum;
    int yAxisValuesSum;
    int xxSum;
    int xySum;

};
