/*
  Example of how to use

public class Statistics
{
    public Trendline CalculateLinearRegression(int[] values)
    {
        var yAxisValues = new List<int>();
        var xAxisValues = new List<int>();

        for (int i = 0; i < values.Length; i++)
        {
            yAxisValues.Add(values[i]);
            xAxisValues.Add(i + 1);
        }

        return new Trendline(yAxisValues, xAxisValues);
    }
}
*/
#include "trendline.h"

Trendline::Trendline(QList<int> yAxisValues, QList<int> xAxisValues)
{
        yAxisValues = yAxisValues;
        xAxisValues = xAxisValues;

        Initialize();
}

void Trendline::Initialize()
{
    count = yAxisValues.count();
    yAxisValuesSum = yAxisValues.size(); //.Sum(); TODO
    xAxisValuesSum = xAxisValues.size(); //.Sum();
    xxSum = 0;
    xySum = 0;

    for (int i = 0; i < count; i++)
    {
        xySum += (xAxisValues[i]*yAxisValues[i]);
        xxSum += (xAxisValues[i]*xAxisValues[i]);
    }

    slope = CalculateSlope();
    intercept = CalculateIntercept();
    start = CalculateStart();
    end = CalculateEnd();
}

int Trendline::CalculateSlope()
{
/*    try
    {*/
        return ((count*xySum) - (xAxisValuesSum*yAxisValuesSum))/((count*xxSum) - (xAxisValuesSum*xAxisValuesSum));
/*     }
    catch (Exception)
    {
        return 0;
    }*/
}

int Trendline::CalculateIntercept()
{
    return (yAxisValuesSum - (slope*xAxisValuesSum))/count;
}

int Trendline::CalculateStart()
{
    return (slope*xAxisValues.first()) + intercept;
}

int Trendline::CalculateEnd()
{
    return (slope*xAxisValues.last()) + intercept;
}
