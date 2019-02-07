#include "light.h"

#define EPSILON 1e-4f
bool PointLight::visibile(Intersection& i, Scene& s, Color3f& hitcolor) {
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
    float closest_t = t;
    bool hitAnything_opaque = false;
    bool hitAnything_trasparent = false;
    //might have problem to use auto with unique_ptr, not sure
    for(const unique_ptr<Primitive>& p : s.primitives) {
        if(p->getIntersection(temp,temp_i)) {
            if(temp_i.t < closest_t && !temp_i.objectHit->material->pass_shadow) {
                hitAnything_opaque = true;
                return false; //cast black shadow
            }
            else if(temp_i.t < closest_t && temp_i.objectHit->material->pass_shadow) {
//                hitAnything_trasparent = true;
//                hitcolor = hue * intensity * temp_i.objectHit->material->base_col; //light with base color of trasparent stuff
//                closest_t = temp_i.t;
            }
        }
    }
    //if we hit transparent object, we cast its base color as the shadow's color
    if(hitAnything_trasparent) {
        return false;
    }
    else
        return true;
}
