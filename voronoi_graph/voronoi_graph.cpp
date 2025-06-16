#include "voronoi_graph.h"
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <iostream>
#include "../dcr/dcr.h"
#include "../yardtree/yardtree.h"

using std::cout;
using std::endl;

struct pair_hash { size_t operator()(const std::pair<int,int>& p) const noexcept {
        return std::hash<long long>()(((long long)p.first<<32) ^ (unsigned long long)p.second);
    }};


Point circumcenter(const Point& A, const Point& B, const Point& C) {
    double D = 2 * (A.x * (B.y - C.y) + B.x * (C.y - A.y) + C.x * (A.y - B.y));
    if (std::abs(D) < 1e-9) {
        return {1e9, 1e9};
    }

    double A2 = A.x * A.x + A.y * A.y;
    double B2 = B.x * B.x + B.y * B.y;
    double C2 = C.x * C.x + C.y * C.y;

    double Ux = (A2 * (B.y - C.y) + B2 * (C.y - A.y) + C2 * (A.y - B.y)) / D;
    double Uy = (A2 * (C.x - B.x) + B2 * (A.x - C.x) + C2 * (B.x - A.x)) / D;

    return {Ux, Uy};
}

const int INF1 = -1000001, INF2 = -1000002, INF3 = -1000003, INF4 = -1000004;
const double BOUND = 1e9;

void VoronoiGraph::insertSite(const Point& newPoint, int newSiteId) {
    // Step 1: Найти ближайшую существующую ячейку
    // cout << "step 1" << endl;
    Point nearest = dnn.nearest(newPoint);
    int start = -1;
    for (auto* cell : cells) {
        if (cell->site.x == nearest.x && cell->site.y == nearest.y) {
            start = cell->siteIndex;
            break;
        }
    }

    dnn.insert(newPoint);

    // Step 2: Если это первая вставка — построить bounding box
    // cout << "step 2" << endl;
    if (start == -1) {
        VoronoiCell* cell = new VoronoiCell(newSiteId);
        cell->site = newPoint;
        std::vector<Point> box = {
                {BOUND, BOUND}, {-BOUND, BOUND},
                {-BOUND, -BOUND}, {BOUND, -BOUND}
        };
        std::vector<int> ids = {INF1, INF2, INF3, INF4, INF1};
        for (int i = 0; i < 4; ++i) {
            VoronoiVertex* v = new VoronoiVertex(newSiteId, ids[i], ids[i+1], box[i]);
            addVertex(v);
            cell->addBoundaryVertex(v);
            boundaryBoxVertices[ids[i]].push_back(v);
            if (i > 0) {
                v->addNeighbor(getVertexById(nextVertexId - 2));
            }
        }
        getVertexById(nextVertexId - 4)->addNeighbor(getVertexById(nextVertexId - 1));
        addCell(cell);
        return;
    }

    // Step 3: BFS по ячейкам — найти затронутые
    // cout << "step 3" << endl;
    std::queue<int> q;
    std::unordered_set<int> visited, affected;
    q.push(start); visited.insert(start);

    while (!q.empty()) {
        int cid = q.front(); q.pop();
        VoronoiCell* cell = getCellById(cid);

        bool hit = false;
        for (auto* v : cell->boundary) {
            Point A = getSitePoint(v->sites[0]);
            Point B = getSitePoint(v->sites[1]);
            Point C = getSitePoint(v->sites[2]);
            if (inCircle(A, B, C, newPoint) > 0) {
                hit = true;
                break;
            }
        }

        if (!hit) continue;

        affected.insert(cid);
        for (auto* v : cell->boundary) {
            for (int site : v->sites) {
                if (site >= 0 && !visited.count(site)) {
                    visited.insert(site);
                    q.push(site);
                }
            }
        }
    }

    // Step 4: Удалить внутренние VoronoiVertex
    // cout << "step 4" << endl;
    std::unordered_set<int> toRemove;
    for (auto* v : vertices) {
        if (affected.count(v->sites[0]) &&
            affected.count(v->sites[1]) &&
            affected.count(v->sites[2])) {
            toRemove.insert(v->id);
        }
    }
    for (int id : toRemove) {
        removeVertex(getVertexById(id));
    }

    // Step 5: Построить новую границу
    // cout << "step 5" << endl;
    VoronoiCell* newCell = new VoronoiCell(newSiteId);
    newCell->site = newPoint;

    std::unordered_set<std::pair<int, int>, pair_hash> used;

    // Проходим по границам INF-сайтов — чтобы "разрывать" bounding box
    for (auto& [infSite, vec] : boundaryBoxVertices) {
        for (auto* v : vec) {
            for (int i = 0; i < 3; ++i) {
                int s = v->sites[i];
                if (s == newSiteId || affected.count(s)) continue;
                auto key = std::minmax(infSite, s);
                if (!used.insert(key).second) continue;

                Point A = getSitePoint(v->sites[0]);
                Point B = getSitePoint(v->sites[1]);
                Point C = getSitePoint(v->sites[2]);
                Point cc = circumcenter(A, B, C);

                VoronoiVertex* newV = new VoronoiVertex(newSiteId, s, infSite, cc);
                addVertex(newV);
                newCell->addBoundaryVertex(newV);

                if (s >= 0) {
                    VoronoiCell* neighbor = getCellById(s);
                    if (neighbor) {
                        neighbor->addBoundaryVertex(newV);
                    }
                } else {
                    boundaryBoxVertices[s].push_back(newV);
                }
            }
        }
    }

    // Стандартный Step 5 — для affected ячеек
    // cout << "step 5" << endl;
    for (int aid : affected) {
        VoronoiCell* cell = getCellById(aid);
        for (auto* v : cell->boundary) {
            for (int i = 0; i < 3; ++i) {
                int s = v->sites[i];
                if (s == newSiteId || affected.count(s)) continue;

                auto key = std::minmax(aid, s);
                if (!used.insert(key).second) continue;

                Point A = getSitePoint(v->sites[0]);
                Point B = getSitePoint(v->sites[1]);
                Point C = getSitePoint(v->sites[2]);
                Point cc = circumcenter(A, B, C);

                VoronoiVertex* newV = new VoronoiVertex(newSiteId, s, aid, cc);
                addVertex(newV);
                newCell->addBoundaryVertex(newV);

                if (s >= 0) {
                    VoronoiCell* neighbor = getCellById(s);
                    if (neighbor) {
                        neighbor->addBoundaryVertex(newV);
                    }
                } else {
                    boundaryBoxVertices[s].push_back(newV);
                }
            }
        }
    }

    addCell(newCell);

    // Step 6: Обновить размеры
    // cout << "step 6" << endl;
    for (int sid : affected) {
        getCellById(sid)->updateSize(N);
    }
    newCell->updateSize(N);
}


// === Вспомогательные методы ===
Point VoronoiGraph::getSitePoint(int siteId) {
    if (siteId == INF1) return { BOUND,  BOUND};
    if (siteId == INF2) return {-BOUND,  BOUND};
    if (siteId == INF3) return {-BOUND, -BOUND};
    if (siteId == INF4) return { BOUND, -BOUND};

    for (auto cell : cells) {
        if (cell->siteIndex == siteId) {
            return cell->site;
        }
    }
    return {0, 0};
}

VoronoiCell* VoronoiGraph::getCellById(int siteId) {
    for (auto cell : cells) {
        if (cell->siteIndex == siteId) return cell;
    }
    return nullptr;
}

VoronoiVertex* VoronoiGraph::getVertexById(int vid) {
    for (auto v : vertices) {
        if (v->id == vid) return v;
    }
    return nullptr;
}
