#include "scene.h"

bool Scene::getIntersection (const Ray& r, Intersection& i) {
    float closest_t = FLT_MAX;
    bool hitAnything = false;
    //temporary intersection object
    Intersection temp_i;
    //might have problem to use auto with unique_ptr, not sure
    for(const unique_ptr<Primitive>& p : primitives) {
        if(p->getIntersection(r,temp_i)) {
            if(temp_i.t < closest_t) {
                hitAnything = true;
                closest_t = temp_i.t;
                i = temp_i;
            }
        }
    }
    return hitAnything;
}
