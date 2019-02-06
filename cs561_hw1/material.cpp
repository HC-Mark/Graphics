#include "material.h"
#define EPSILON 1e-4f
Color3f LambertianMaterial::GetScatteredColor(Intersection& i, Ray& r, Scene& s,int curr_depth) {

    //test with all lights in scene to get the
    Color3f hitcolor = glm::vec3(0,0,0);
    for(int j = 0; j < s.lights.size(); ++j) {
        //if visible, add light to
        if(s.lights[j]->visibile(i,s)) {
            //compute the incident light angle
            Vector3f L = glm::normalize(s.lights[j]->position - i.point);
            //calculate cos theta
            float cos_theta = std::min(1.0f, std::max(0.f, glm::dot(i.normal,L))); //shouldn't be smaller than 1
            //this step calculate the color contributed by this light source, which is the light color applied on the base color of the object
            hitcolor += (base_col * s.lights[j]->intensity * s.lights[j]->hue * cos_theta) / (s.lights.size() * 1.0f);
            //since it doesn't matter where the observer is, the output color should be the same for any eye in the coordinate
        }
    }

    return hitcolor;
}
//too shiny
Color3f PhongMaterial::GetScatteredColor(Intersection& i, Ray& r, Scene& s,int curr_depth) {

    //test with all lights in scene to get the
    Color3f hitcolor = glm::vec3(0,0,0);
    for(int j = 0; j < s.lights.size(); ++j) {
        //if visible, add light to
        //kd and ks
        float kd = 0.6; //without this coefficient will cause the surface too shiny
        float ks = 0.4;
        if(s.lights[j]->visibile(i,s)) {
            //compute the incident light angle
            Vector3f L = glm::normalize(s.lights[j]->position - i.point);
            //calculate cos theta
            float cos_theta = std::min(1.0f, std::max(0.f, glm::dot(i.normal,L))); //shouldn't be smaller than 1
            //this step calculate the color contributed by this light source, which is the light color applied on the base color of the object
            //diffuse part of light the same as lambert
            hitcolor += (base_col * s.lights[j]->intensity * s.lights[j]->hue * cos_theta) / (s.lights.size() * 1.0f) * kd;

            //specular component
            //float exp = 10; //exponential of phong model, which should be an input of constructor I think
           //calculate the ideal reflection direction for this ray
            Vector3f reflect_dirt = (-L) - 2 * glm::dot((-L),i.normal) * i.normal;
           //calculate the view ray direction
            Vector3f view_dirt = glm::normalize(s.camera.e - i.point);
            //do we need to consider the light source number? I think no
            hitcolor += glm::max(s.lights[j]->intensity * std::pow(std::max(glm::dot(reflect_dirt,view_dirt),0.f),specular_power), glm::vec3(0,0,0)) * ks;
            //hitcolor = view_dirt;
        }
    }

    return hitcolor;
}

Color3f SpecularReflectionMaterial::GetScatteredColor(Intersection& i, Ray& r, Scene& s,int curr_depth) {

    if(curr_depth < max_depth) {
        //calculate reflection direction
        for(int j = 0; j < s.lights.size(); ++j) {
            Vector3f L = glm::normalize(s.lights[j]->position - i.point);
            Vector3f reflect_dirt = (-L) - 2 * glm::dot((-L),i.normal) * i.normal;
            //update the ray and intersection point
            //create new ray need to consider shadow acne
            Ray temp_r(i.point + EPSILON * i.normal,reflect_dirt);
            Intersection temp_i = i;
            //whether new ray hits anything
            if(s.getIntersection(temp_r,temp_i)) {
                //how to combine the base color with the reflected object?
                //what is specular color?
                Color3f hitcolor = temp_i.objectHit->material->GetScatteredColor(temp_i,temp_r,s,curr_depth + 1);
                hitcolor
            }
        }
    }

    return Color3f(0,0,0);

}

//Color3f SpecularReflectionMaterial::GetScatteredColor_rec(Intersection& i, Ray& r, Scene& s,int curr_depth) {
//    //how should I pass the current level
//    if(curr_depth < max_depth) {
//        Intersection temp_i = i;
//        if(s.getIntersection(r,temp_i)) {
//             return temp_i.objectHit->material->GetScatteredColor(temp_i,r,s);
//        }
//    }
//    else
//        return Color3f(0,0,0);
//}
