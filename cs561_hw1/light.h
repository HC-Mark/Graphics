#ifndef LIGHT_H
#define LIGHT_H
//#include "globalincludes.h"
#include "intersection.h"
#include "scene.h"

class Scene;
class Intersection;

class Light {
public:
    Color3f hue;
    Color3f intensity;
    Point3f position;
    //do we need to seperate hue and intensity?

    Light() : hue(glm::vec3(0,0,0)),intensity(glm::vec3(0,0,0)),position(glm::vec3(0,0,0)) {}
    Light(Color3f h, Color3f i, Transform t) : hue(h), intensity(i) {
        glm::vec4 position_raw(0,0,0,1); //transform origin to correspond positions
        position = glm::vec3(t.get_worldTransform() * position_raw);
    }

    virtual bool visibile(Intersection& i, Scene& s, Color3f& hitcolor) = 0;
};

class PointLight : public Light {
public:
    //Point3f position; //we can use position or transform, but I think transform might be better since we can use both in point light and area light
    //where should we place the position of light? should it be in abstract class or in
    //constructor
    PointLight() : Light(){}
    PointLight(Color3f h, Color3f i, Transform t) : Light(h,i,t) {}

    bool visibile(Intersection &i, Scene &s, Color3f& hitcolor);
};

#endif // LIGHT_H
