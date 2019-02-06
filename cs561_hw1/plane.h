#ifndef PLANE_H
#define PLANE_H
#include "shape.h"

class Plane : public Shape {
public:
    float length;
    Point3f origin;
    Vector3f norm;

    //constructors
    Plane(): Shape(),length(1.f), origin(glm::vec3(0,0,0)), norm(glm::vec3(0,0,1)) {}
    Plane(Transform t) : Shape(t), length(1.f), origin(glm::vec3(0,0,0)),norm(glm::vec3(0,0,1)) {}
    Plane(Transform t, float l, Point3f o, Vector3f n): Shape(t), length(l), origin(o), norm(n) {}

    //derived destructor
    virtual ~Plane() {}
    //getIntersection
    bool getIntersection(const Ray &r, Intersection &i);

};
#endif // PLANE_H
