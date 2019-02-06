#ifndef MATERIAL_H
#define MATERIAL_H
#include "intersection.h"
#include "scene.h"

class Scene;

class Material {
public:
    Color3f base_col;
    Material(Color3f c) : base_col(c) {}
    virtual Color3f GetScatteredColor(Intersection& i, Ray& r, Scene& s, int cur_depth) = 0;
};

class LambertianMaterial : public Material {
public:
    //constructor
    LambertianMaterial(Color3f c) : Material(c) {}

    Color3f GetScatteredColor(Intersection& i, Ray& r, Scene& s,int curr_depth);
};

class PhongMaterial : public Material {
public:
    //constructor
    float specular_power;
    PhongMaterial(Color3f c, float sp_power) : Material(c), specular_power(sp_power) {}

    Color3f GetScatteredColor(Intersection& i, Ray& r, Scene& s,int curr_depth);
};

class SpecularReflectionMaterial : public Material {
public:
    //constructor
    int max_depth;
    SpecularReflectionMaterial(Color3f c, int m) : Material(c), max_depth(m) {}

    Color3f GetScatteredColor(Intersection& i, Ray& r, Scene& s,int curr_depth);
    //Color3f GetScatteredColor_rec(Intersection& i, Ray& r, Scene& s,int curr_depth);
};


class SpecularTransmissionMaterial : public Material {
public:
    //constructor
    int max_depth;
    SpecularTransmissionMaterial(Color3f c, int m) : Material(c), max_depth(m) {}

    Color3f GetScatteredColor(Intersection& i, Ray& r, Scene& s,int curr_depth);
};
#endif // MATERIAL_H
