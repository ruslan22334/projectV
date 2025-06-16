//#define _GLIBCXX_DEBUG

#include "voronoi_graph/voronoi_graph.h"
#include "geometry/geometry.h"
#include <iostream>
#include <random>
#include "dnn/stupid_dnn.h"
#include "assert.h"
#include <chrono>

void testDNN() {
    // Test DNN against a naive implementation
    int timeDnn = 0;
    int timeStupid = 0;
    // нужно замерить суммарное время
    // работы обоих алгоритмов
    const int T = 10000; // Number of points to insert
    std::vector <Point> points;
    std::vector <Point> queries;
    for (int i=0; i<T; i++){
        double x = static_cast<double>(rand()) / RAND_MAX * 100.0; // Random x in [0, 100]
        double y = static_cast<double>(rand()) / RAND_MAX * 100.0; // Random y in [0, 100]
        points.push_back({x, y});
        x = static_cast<double>(rand()) / RAND_MAX * 100.0; // Random x in [0, 100]
        y = static_cast<double>(rand()) / RAND_MAX * 100.0; // Random y in [0, 100]
        queries.push_back({x, y});
    }
    std::cout << "first 10 points in test: " << std::endl;
    for (int i=0; i<10; i++){
        std::cout << "(" << points[i].x << "," << points[i].y << ") ; ";
    }
    std::cout << std::endl;

    std::vector <Point> ansDnn;
    std::vector <Point> ansStupid;

    DNN dnn;
    stupid_dnn stupidDnn;
    // Insert points into DNN
    auto startDnn = std::chrono::high_resolution_clock::now();
    for (int i=0; i<T; i++){
        dnn.insert(points[i]);
        ansDnn.push_back(dnn.nearest(queries[i]));
    }
    auto endDnn = std::chrono::high_resolution_clock::now();
    timeDnn = std::chrono::duration_cast<std::chrono::milliseconds>(endDnn - startDnn).count();
    // Insert points into stupid DNN
    auto startStupid = std::chrono::high_resolution_clock::now();
    for (int i=0; i<T; i++){
        stupidDnn.insert(points[i]);
        ansStupid.push_back(stupidDnn.nearest(queries[i]));
    }
    auto endStupid = std::chrono::high_resolution_clock::now();
    timeStupid = std::chrono::duration_cast<std::chrono::milliseconds>(endStupid - startStupid).count();
    // Check if results are the same
    for (int i=0; i<T; i++){
        assert(ansDnn[i].x == ansStupid[i].x && ansDnn[i].y == ansStupid[i].y);
    }
    std::cout << "DNN number of test points = " << T << std::endl;
    std::cout << "DNN time is " << timeDnn << " ms" << std::endl;
    std::cout << "Stupid DNN time is " << timeStupid << " ms" << std::endl;

    std::cout << "\nbig test time check" << std::endl;
    const int t = 100000;
    std::cout << "big test number of points = " << t << std::endl;
    DNN d;
    startDnn = std::chrono::high_resolution_clock::now();
    for (int i=0; i<t; i++){
        double x = static_cast<double>(rand()) / RAND_MAX * 100.0; // Random x in [0, 100]
        double y = static_cast<double>(rand()) / RAND_MAX * 100.0; // Random y in [0, 100]
        d.insert({x,y});
        x = static_cast<double>(rand()) / RAND_MAX * 100.0; // Random x in [0, 100]
        y = static_cast<double>(rand()) / RAND_MAX * 100.0; // Random y in [0, 100]
        d.nearest({x,y});
    }
    endDnn = std::chrono::high_resolution_clock::now();
    timeDnn = std::chrono::duration_cast<std::chrono::milliseconds>(endDnn - startDnn).count();
    std::cout << "DNN time is " << timeDnn << " ms" << std::endl;

    std::cout << "DNN ok" << std::endl;
}


int main() {
    std::cout << "DNN test" << std::endl;
    testDNN();

    std::cout << "Voronoi Graph test" << std::endl;
    VoronoiGraph graph;

    std::mt19937 rng(42);
    std::uniform_real_distribution<double> distX(-50.0, 50.0);
    std::uniform_real_distribution<double> distY(-50.0, 50.0);

    int numSites = 5;
    for (int i = 1; i <= numSites; ++i) {
        Point p = {distX(rng), distY(rng)};
        std::cout << "Inserting site " << i << " at (" << p.x << ", " << p.y << ")\n";
        graph.insertSite(p, i);
    }

    std::cout << "Total cells: " << graph.cells.size() << std::endl;
    std::cout << "Total vertices: " << graph.vertices.size() << std::endl;

    // Для каждой ячейки выводим границу
    for (const auto& cell : graph.cells) {
        std::cout << "Cell for site " << cell->siteIndex << ": ";
        std::cout << "boundary has " << cell->boundary.size() << " vertices.\n";

        for (const auto& v : cell->boundary) {
            std::cout << "  VoronoiVertex sites=("
                      << v->sites[0] << ", "
                      << v->sites[1] << ", "
                      << v->sites[2] << ") "
                      << "pos=(" << v->pos.x << ", " << v->pos.y << ")\n";

        }
    }

    return 0;
}
