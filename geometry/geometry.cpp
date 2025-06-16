#include "geometry.h"


double orient2d(const Point& a, const Point& b, const Point& c) {
    return (b.x - a.x)*(c.y - a.y) - (b.y - a.y)*(c.x - a.x);
}

double inCircle(const Point& a, const Point& b, const Point& c, const Point& d) {
    double adx = a.x - d.x;
    double ady = a.y - d.y;
    double bdx = b.x - d.x;
    double bdy = b.y - d.y;
    double cdx = c.x - d.x;
    double cdy = c.y - d.y;

    double ad2 = adx*adx + ady*ady;
    double bd2 = bdx*bdx + bdy*bdy;
    double cd2 = cdx*cdx + cdy*cdy;

    double det = adx * (bdy*cd2 - bd2*cdy)
                 - ady * (bdx*cd2 - bd2*cdx)
                 + ad2 * (bdx*cdy - bdy*cdx);

    return det;
}
