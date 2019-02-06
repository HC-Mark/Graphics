#include "plane.h"

bool Plane::getIntersection(const Ray &r, Intersection &i) {
    //apply inverseTransform to ray
    Ray temp = r;
    temp.trans_ray(transform.get_worldTransformInverse());
    float denom = glm::dot(norm,temp.direct);
    //if they are not parallel, they must intersect
    //avoid to divide by 0
    if(abs(denom) > 0.000001) {
        Vector3f line_on_plane = origin - temp.origin;
        float t = glm::dot(line_on_plane,norm) / denom;
        if(t > 0) {
            //calculate the intersection point and its distance with plane center
            Point3f p = temp.origin + t * temp.direct;
            if(abs(p.x) <= 0.5 && abs(p.y) <= 0.5){
                i.t = t;
                //wrong position, we use the transformed point as intersection point in world space, it is wrong
                i.point = r.origin + r.direct * t;
                //normal was not normalized
                i.normal = glm::normalize(transform.get_worldTransformInverseTranspose() * norm);
                return true;
            }
        }
    }
    return false;
}
