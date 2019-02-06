#include "primitive.h"

bool Primitive::getIntersection(const Ray& r, Intersection& i) {
    //call the shape stored in this primitive to check intersection
    if(intersectable->getIntersection(r,i)){
        //we can't assign primitive to the intersection in a shape, so we do it here
        i.objectHit = this;
        return true;
    }
    return false;
}
