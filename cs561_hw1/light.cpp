#include "light.h"

#define EPSILON 1e-4f
bool PointLight::visibile(Intersection& i, Scene& s) {
    //what I am thinking about constructing this function is like this
    //we return the shadow ray of the intersection point and the this light source
    //then in scene construction, we us the ray and compute the intersection with each primitive in p list

    //how should we know the position of light?  --set up a position
    //how should we get the position of other primitive -- pass in the scene

    //form a ray from intersection point to pointlight, and check intersection of that ray
    Vector3f direction  = glm::normalize(position - i.point);
    float t = glm::length(position - i.point);
    Ray temp((i.point +  EPSILON * i.normal),direction); //from intersection point to light source
    Intersection temp_i; //shouldn't change i
    //check intersection between shadow ray and objects in scene
    if(s.getIntersection(temp,temp_i)) {
        //it should set up a standard t such that only when the object's hitting point's t value smaller than this t, it counts
        if(temp_i.t < t)
            return false; //not visible, can be optimized
    }

    return true;
}
