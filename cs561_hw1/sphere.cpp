#include "sphere.h"

bool Sphere::getIntersection(const Ray &r, Intersection &i) {
    //apply inverse
    Ray temp_r = r;
    temp_r.trans_ray(transform.get_worldTransformInverse());
    //center is always (0,0,0)
    float a = glm::dot(temp_r.direct,temp_r.direct);
    float b = 2.f * glm::dot(temp_r.origin,temp_r.direct);
    float c = glm::dot(temp_r.origin,temp_r.origin) - radius * radius;

    float discriminant = b*b - 4.f*a*c;
    //if the ray intersects with sphere, the discriminant should > 0
    if(discriminant > 0) {
        //calculate the two result and make sure they are in front of the camera
        float temp = (-b - sqrt(discriminant))/(2.f*a);
        //object space intersection point
        Point3f int_p = temp_r.origin + temp * temp_r.direct;
        if(temp > 0) {
            i.t = temp;
            i.point = r.origin + temp * r.direct;
            i.normal = glm::normalize(transform.get_worldTransformInverseTranspose() * int_p);
            //i.objectHit = this;
            return true;
        }

        temp = (-b + sqrt(discriminant))/(2.f*a);
        int_p = temp_r.origin + temp * temp_r.direct;
        if(temp > 0) {
            i.t = temp;
            i.point = r.origin + temp * r.direct; //in world space, not object space
            i.normal = glm::normalize(transform.get_worldTransformInverseTranspose() * int_p);
            //i.objectHit = this;
            return true;
        }
    }

    return false;
}
