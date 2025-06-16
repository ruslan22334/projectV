#include "dcr.h"
#include <algorithm>

Circle::Circle(const Point& p1, const Point& p2, const Point& p3)
        : a(p1), b(p2), c(p3) {
    double cross = (b.x - a.x)*(c.y - a.y) - (b.y - a.y)*(c.x - a.x);
    if (cross < 0) {
        std::swap(b, c);
    }
}

bool Circle::contains(const Point& p) const {
    double adx = a.x - p.x, ady = a.y - p.y;
    double bdx = b.x - p.x, bdy = b.y - p.y;
    double cdx = c.x - p.x, cdy = c.y - p.y;

    double alift = adx*adx + ady*ady;
    double blift = bdx*bdx + bdy*bdy;
    double clift = cdx*cdx + cdy*cdy;

    double abdet = adx * bdy - bdx * ady;
    double bcdet = bdx * cdy - cdx * bdy;
    double cadet = cdx * ady - adx * cdy;

    double det = alift * bcdet + blift * cadet + clift * abdet;
    return det >= 0;
}

void DCR::insertCircle(const Point& a, const Point& b, const Point& c) {
    circles.emplace_back(a, b, c);
}

bool DCR::removeCircle(const Point& a, const Point& b, const Point& c) {
    Circle target(a, b, c);
    for (auto it = circles.begin(); it != circles.end(); ++it) {
        if (it->a.x == target.a.x && it->a.y == target.a.y &&
            it->b.x == target.b.x && it->b.y == target.b.y &&
            it->c.x == target.c.x && it->c.y == target.c.y) {
            circles.erase(it);
            return true;
        }
    }
    return false;
}

std::vector<Circle> DCR::query(const Point& p) const {
    std::vector<Circle> result;
    for (const auto& circle : circles) {
        if (circle.contains(p)) {
            result.push_back(circle);
        }
    }
    return result;
}
