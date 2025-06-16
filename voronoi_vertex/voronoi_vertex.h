#pragma once
#include <array>
#include <vector>
#include <algorithm>
#include "../geometry/geometry.h"

struct VoronoiVertex {
    std::vector<int> sites;
    std::vector<VoronoiVertex*> nbrs;
    int id;

    VoronoiVertex(int s1, int s2, int s3, const Point& p)
            : pos(p) {
        sites = {s1, s2, s3};
        nbrs.reserve(3);
    }

    void addNeighbor(VoronoiVertex* v) { nbrs.push_back(v); }
    void removeNeighbor(VoronoiVertex* v) {
        nbrs.erase(std::remove(nbrs.begin(), nbrs.end(), v), nbrs.end());
    }

    Point pos;
};