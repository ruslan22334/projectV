#pragma once
#include <vector>
#include "../geometry/geometry.h"

struct Circle {
    Point a, b, c;
    Circle(const Point& p1, const Point& p2, const Point& p3);
    bool contains(const Point& p) const;
};

class DCR {
public:
    DCR() = default;

    void insertCircle(const Point& a, const Point& b, const Point& c);
    bool removeCircle(const Point& a, const Point& b, const Point& c);
    std::vector<Circle> query(const Point& p) const;

private:
    std::vector<Circle> circles;
};
