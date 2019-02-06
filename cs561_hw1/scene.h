#ifndef SCENE_H
#define SCENE_H
#include "camera.h"
#include "primitive.h"
#include "light.h"

class Light;
class Primitive;
using std::unique_ptr;
using std::vector;
class Scene {
public:
    Camera camera;
    //can't use QList
    //QList<unique_ptr<Primitive>> primitives;
    vector<unique_ptr<Primitive>> primitives;
    vector<unique_ptr<Light>> lights;
    Scene(Camera c, vector<unique_ptr<Primitive>>& ps, vector<unique_ptr<Light>>& ls) : camera(c) {
        //need a loop to trnasfer ownership
        int p_size = ps.size(), l_size = ls.size();
        for(int i = 0; i < p_size; ++i) {
            primitives.push_back(std::move(ps[i]));
        }

        for(int i = 0; i < l_size; ++i) {
            lights.push_back(std::move(ls[i]));
        }
    }

    ~Scene(){}
    bool getIntersection (const Ray& r, Intersection& i);

};
#endif // SCENE_H
