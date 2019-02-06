#ifndef PRIMITIVE_H
#define PRIMITIVE_H
#include "camera.h"
#include "shape.h"
#include "material.h"

using std::unique_ptr;

class Shape;
class Intersection;
class Material;

class Primitive {
public:
    QString name;
    unique_ptr<Shape> intersectable;
    unique_ptr<Material> material;

    //Primitive(): name("NULL"), intersectable(unique_ptr<Shape>(new Shape())){}
    Primitive(QString n, Shape* s, Material* m): name(n), intersectable(unique_ptr<Shape>(s)), material(unique_ptr<Material>(m)) {}
    //question, if we already have a shape in raw pointer, why we use a unique_ptr in primitive
    bool getIntersection(const Ray& r, Intersection& i);
};


#endif // PRIMITIVE_H
