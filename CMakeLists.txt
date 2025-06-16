cmake_minimum_required(VERSION 3.22)
project(project)

set(CMAKE_CXX_STANDARD 17)

add_executable(project main.cpp
        dcr/dcr.cpp
        dcr/dcr.h
        voronoi_graph/voronoi_graph.cpp
        voronoi_graph/voronoi_graph.h
        voronoi_cell/voronoi_cell.cpp
        voronoi_cell/voronoi_cell.h
        voronoi_vertex/voronoi_vertex.cpp
        voronoi_vertex/voronoi_vertex.h
        geometry/geometry.cpp
        geometry/geometry.h
        yardtree/yardtree.cpp
        yardtree/yardtree.h
        dnn/dnn.cpp
        dnn/dnn.h
        dnn/stupid_dnn.cpp
        dnn/stupid_dnn.h
)
