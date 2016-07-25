#ifndef MATHUTILITIES
#define MATHUTILITIES

#include <cmath>
#include <QPoint>
#include <QLine>

namespace MathUtilities {

    double length(const QPoint &a, const QPoint &b)
    {
        return std::sqrt((a.x()-b.x())*(a.x()-b.x()) + (a.y()-b.y())*(a.y()-b.y()));
    }

    bool isOnSegment(const QPoint &a, const QPoint &b, const QPoint &c, double eps = 0.1)
    {

        double ab = length(a,b);
        double ac = length(a,c);
        double cb = length(c,b);
        return std::fabs(ac + cb - ab) < eps;
    }
}
#endif // MATHUTILITIES

