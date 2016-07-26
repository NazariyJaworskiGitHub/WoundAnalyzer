#ifndef MATHUTILITIES
#define MATHUTILITIES

#include <cmath>
#include <QPointF>

namespace MathUtilities {

    double length(const QPointF &a, const QPointF &b)
    {
        return std::sqrt((a.x()-b.x())*(a.x()-b.x()) + (a.y()-b.y())*(a.y()-b.y()));
    }

    bool isOnSegment(const QPointF &a, const QPointF &b, const QPointF &c, double eps = 0.1)
    {

        double ab = length(a,b);
        double ac = length(a,c);
        double cb = length(c,b);
        return std::fabs(ac + cb - ab) < eps;
    }
}
#endif // MATHUTILITIES

