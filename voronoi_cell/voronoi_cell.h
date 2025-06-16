#pragma once
#include <vector>
#include "../voronoi_vertex/voronoi_vertex.h"
#include "../dcr/dcr.h"
#include "../yardtree/yardtree.h"
struct VoronoiCell {
    int siteIndex;
    Point site;
    std::vector<VoronoiVertex*> boundary;
    bool isBig;
    DCR dcr;
    YardTree yardTree;
    VoronoiCell(int s) : siteIndex(s), isBig(false) {}
    void addBoundaryVertex(VoronoiVertex* v) { boundary.push_back(v); }
    void updateSize(int M) { isBig = (boundary.size() > (size_t)M); }
};