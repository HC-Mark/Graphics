#ifndef SPHERE_H
#define SPHERE_H
#include "shape.h"

class Sphere : public Shape {
public:
    float radius;
    Point3f origin;

    //constructors
    Sphere(): Shape(),radius(0.5), origin(glm::vec3(0,0,0)) {}
    Sphere(Transform t) : Shape(t), radius(0.5), origin(glm::vec3(0,0,0)) {}
    Sphere(Transform t, float r, Point3f o): Shape(t), radius(r), origin(o) {}

    //derived destructor
    virtual ~Sphere() {}
    //getIntersection
    bool getIntersection(const Ray &r, Intersection &i);

};
#endif // SPHERE_H
