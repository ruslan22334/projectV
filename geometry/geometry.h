#pragma once
#include <cmath>

struct Point {
    double x, y;

    Point(double x_ = 0.0, double y_ = 0.0) : x(x_), y(y_) {}

    Point operator-(const Point& other) const {
        return {x - other.x, y - other.y};
    }

    double dot(const Point& other) const {
        return x * other.x + y * other.y;
    }
};

double orient2d(const Point& a, const Point& b, const Point& c);
double inCircle(const Point& a, const Point& b, const Point& c, const Point& d);
