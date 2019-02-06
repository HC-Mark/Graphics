#ifndef RAY_H
#define RAY_H
#include "globalincludes.h"
#include <iostream>

class Ray {
public:
    Point3f origin;
    Vector3f direct;
    Ray() : origin(Point3f(0,0,0)), direct(Vector3f(0,0,0)) {}
    Ray(Point3f o, Vector3f d) : origin(o), direct(glm::normalize(d)) {}

    //use a 4d transformation to transform the current ray
    void trans_ray(const glm::mat4& trans_mat);
private:

};
#endif // RAY_H
