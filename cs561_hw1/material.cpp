#include "material.h"
#define EPSILON 1e-4f
Color3f LambertianMaterial::GetScatteredColor(Intersection& i, Ray& r, Scene& s,int curr_depth) {

    //test with all lights in scene to get the
    Color3f hitcolor = glm::vec3(0,0,0);
    for(int j = 0; j < s.lights.size(); ++j) {
        //if visible, add light to
        if(s.lights[j]->visibile(i,s,hitcolor)) {
            //compute the incident light angle
            Vector3f L = glm::normalize(s.lights[j]->position - i.point);
            //calculate cos theta
            float cos_theta = std::min(1.0f, std::max(0.f, glm::dot(i.normal,L))); //shouldn't be smaller than 1
            //this step calculate the color contributed by this light source, which is the light color applied on the base color of the object
            hitcolor += (base_col * s.lights[j]->intensity * s.lights[j]->hue * cos_theta) / (s.lights.size() * 1.0f);
            //since it doesn't matter where the observer is, the output color should be the same for any eye in the coordinate
        }
        else if(hitcolor != glm::vec3(0,0,0)) {
            //if we pass a trasparent object
            Vector3f L = glm::normalize(s.lights[j]->position - i.point);
            //calculate cos theta
            float cos_theta = std::min(1.0f, std::max(0.f, glm::dot(i.normal,L))); //shouldn't be smaller than 1
            //base color of transparent object with the normal color of object
            hitcolor *= (base_col * s.lights[j]->intensity * s.lights[j]->hue * cos_theta) / (s.lights.size() * 1.0f);
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
        if(s.lights[j]->visibile(i,s,hitcolor)) {
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
        float kd = 0.6; //without this coefficient will cause the surface too shiny
        float ks = 0.4;
            Vector3f reflect_dirt = r.direct - 2 * glm::dot(r.direct,i.normal) * i.normal;
            //update the ray and intersection point
            //create new ray need to consider shadow acne
            Ray temp_r(i.point + EPSILON * i.normal,reflect_dirt);
            Intersection temp_i = i;
            //whether new ray hits anything -- assume only one light?
            if(s.getIntersection(temp_r,temp_i)) {
                //how to combine the base color with the reflected object?
                //what is specular color? -- this is only the specualr component of the surface light
                Color3f hitcolor = temp_i.objectHit->material->GetScatteredColor(temp_i,temp_r,s,curr_depth + 1) ;
                return hitcolor * base_col; //become darker
            }
    }
    else
        return base_col;

    return Color3f(0,0,0);

}

Color3f SpecularTransmissionMaterial::GetScatteredColor(Intersection& i, Ray& r, Scene& s,int curr_depth) {
    Color3f hitcolor = glm::vec3(1.0);

    if(curr_depth < max_depth) {
        //for(int j = 0; j < s.lights.size(); ++j) {
            //Vector3f L = glm::normalize(s.lights[j]->position - i.point);
            Vector3f incident = glm::normalize(r.direct);
            //calculate whether the ray is outside or inside object
            bool outside = glm::dot(incident,i.normal) < 0;
            Vector3f surface_normal = i.normal;
            float ior_from = iorOut, ior_to = iorIn;
            if(!outside) {
                ior_from = iorIn;
                ior_to = iorOut;
                //also the normal is the surface normal outside, but the incident ray is inside the surface.
                //we need to inverse the direction of surface normal to make it correct
                surface_normal = -surface_normal;
            }
            float ratio = ior_from / ior_to;
            //calculate the refraction ray
            Vector3f ref_dirt = glm::refract(incident,surface_normal,ratio);
            Point3f ref_origin;
            if (glm::length(ref_dirt) <= 0.0){
                ref_dirt = glm::reflect(incident,surface_normal);
                ref_origin = outside ? i.point + EPSILON * i.normal : i.point - EPSILON * i.normal;
            }
            else {
                ref_origin = outside ? i.point - EPSILON * i.normal : i.point + EPSILON * i.normal;
            }
                Ray ref_ray(ref_origin,glm::normalize(ref_dirt));
                Intersection temp_i;
                if(s.getIntersection(ref_ray,temp_i)) {
                    hitcolor = temp_i.objectHit->material->GetScatteredColor(temp_i,ref_ray,s,curr_depth + 1);
                    //return hitcolor;
                }
        //}
    }
    return hitcolor * base_col;

}

Vector3f SpecularTransmissionMaterial::refract_dirt(Intersection& i, Vector3f light_vec) {
    Vector3f surface_norm = i.normal;
    float cos_theta1 = glm::clamp(glm::dot(light_vec,surface_norm),-1.f,1.f);
    //determine the from and to index of refraction
    float ior_from = iorOut, ior_to = iorIn;
    //determine whether we are inside or outside of material
    if(cos_theta1 < 0) {
        //outside material, ray will refract into material
        cos_theta1 = -cos_theta1; //want positivie cos
    }
    else {
        //ray is inside the material and want to refract to outside
        //we need to reverse the ior from and ior to
        ior_from = iorIn;
        ior_to = iorOut;
        //also the normal is the surface normal outside, but the incident ray is inside the surface.
        //we need to inverse the direction of surface normal to make it correct
        surface_norm = -surface_norm;
    }

    float ratio = ior_from / ior_to; //cos_theta1 / cos_theta2
    //calculate the direction
    float dirDot = glm::dot(glm::normalize(light_vec), glm::normalize(surface_norm));
    float sq_cos_theta2 = 1.f - ratio * ratio * (1 - dirDot * dirDot);
    //determine whehter sq_cos_theta2 is negative, if so we ignore the refrcos_theta1 * cos_theta1act effect, because it doesn't have real number solution
    if(sq_cos_theta2 >= 0) {
        //calculate the direction -- formula from Scratch pixel
        return glm::refract(light_vec, surface_norm, ratio);
        return ratio * light_vec - (ratio * cos_theta1 + sqrt(sq_cos_theta2)) * surface_norm;
        //return ratio * light_vec + (ratio * cos_theta1 - sqrt(sq_cos_theta2)) * surface_norm;
    }
    else
        return glm::vec3(0,0,0);
}
