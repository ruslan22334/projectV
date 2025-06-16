#pragma once
#include "../voronoi_vertex/voronoi_vertex.h"
#include "../voronoi_cell/voronoi_cell.h"
#include "../dnn/dnn.h"
#include "../bigcellgraph/bigcellgraph.h"

struct VoronoiGraph {
    std::vector<VoronoiVertex*> vertices;
    std::vector<VoronoiCell*> cells;
    DNN dnn;
    BigCellGraph bigGraph;
    int N = 4; // threshold for big cell
    void addVertex(VoronoiVertex* v) {
        v->id = nextVertexId++; // ОБЯЗАТЕЛЬНО добавить это поле!
        vertices.push_back(v);
    }

    void addCell(VoronoiCell* c) { cells.push_back(c); }
    void insertSite(const Point& newPoint, int newSiteId);
    Point getSitePoint(int siteId);
    VoronoiCell* getCellById(int siteId);
    VoronoiVertex* getVertexById(int vid);
    void removeVertex(VoronoiVertex* v) {
        for (auto u : v->nbrs) {
            u->removeNeighbor(v);
        }
        vertices.erase(std::remove(vertices.begin(), vertices.end(), v), vertices.end());
        delete v;
    }
    int nextVertexId = 0;
    std::unordered_map<int, std::vector<VoronoiVertex*>> boundaryBoxVertices;
};