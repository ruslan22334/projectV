#pragma once
#include <vector>
#include "../geometry/geometry.h"

class stupid_dnn {
private:
    std::vector <Point> points;
public:
    void insert(const Point& p) {
        points.push_back(p);
    }

    Point nearest(const Point& p) {
        if (points.empty()) return Point();

        Point best = points[0];
        double bestDist = (best.x - p.x) * (best.x - p.x) + (best.y - p.y) * (best.y - p.y);

        for (const auto& point : points) {
            double dist = (point.x - p.x) * (point.x - p.x) + (point.y - p.y) * (point.y - p.y);
            if (dist < bestDist) {
                bestDist = dist;
                best = point;
            }
        }
        return best;
    }
};
