#include <stdlib.h>
#include <iostream>
#include "robotmath.h"

typedef struct {
    Point p;
    const char* color;
} PointPair;

typedef struct{
    Point p;
    Vector v;
    const char* color;
} VectorPair;

class Graph{
public:
    PointPair* points = (PointPair*)malloc(0);
    VectorPair* vectors = (VectorPair*)malloc(0);
    int numOfPoints = 0;
    int numOfVectors = 0;

    ~Graph() {
        free(points);
        free(vectors);
    }

    void clear() {
        free(points);
        free(vectors);
        points = (PointPair*)malloc(0);
        vectors = (VectorPair*)malloc(0);
        numOfPoints = 0;
        numOfVectors = 0;
    }

    void addVector(VectorPair x) {
        numOfVectors++;
        vectors = (VectorPair*)realloc(vectors, sizeof(VectorPair) * numOfVectors);
        vectors[numOfVectors - 1] = x;
    }

    void addPoint(PointPair x) {
        numOfPoints++;
        points = (PointPair*)realloc(points, sizeof(PointPair) * numOfPoints);
        points[numOfPoints - 1] = x;
    }

    void output() {
        for (int i = 0; i < numOfVectors; i++) {
            VectorPair v = vectors[i];
            std::cout << "V:" << v.p.x << "," << v.p.y << "," << v.v.getX() << "," << v.v.getY() << "," << v.color << std::endl;
        }
        for (int i = 0; i < numOfPoints; i++) {
            PointPair p = points[i];
            std::cout << "P:" << p.p.x << "," << p.p.y << "," << p.color << std::endl;
        }
        std::cout << "END:END" << std::endl;
    }
};

Graph graph = Graph();