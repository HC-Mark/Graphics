#ifndef INTERSECTION_H
#define INTERSECTION_H

//what should we include in this header?
#include "primitive.h"

class Primitive;

class Intersection {
public:
    Point3f point;
    Normal3f normal;
    float t;
    Primitive* objectHit;

    Intersection() {}
    Intersection(Point3f p, Normal3f n, float input_t, Primitive* object): point(p), normal(n), t(input_t), objectHit(object) {}
    //what should be the constructor of the intersection? what information we can use to compute all variables
};


#endif // INTERSECTION_H
