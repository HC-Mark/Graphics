#ifndef SHAPE_H
#define SHAPE_H

#include "transform.h"
#include "intersection.h"

class Transform;
class Intersection;
class Shape {
public:
    Transform transform;

    //constructor
    Shape() : transform(Transform()) {}
    Shape(Transform t) : transform(t) {}

    //virtual destructor
    virtual ~Shape() {}

    //virtual getIntersection
    virtual bool getIntersection(const Ray& r, Intersection& i)  =  0;
};

#endif // SHAPE_H
