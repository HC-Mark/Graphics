#include "camera.h"

Camera::Camera(Point3f eye, Point3f ref_p,Vector3f world_up, float fov, float far, float near, int width, int height) {
    //calculate the view matrix first
    float tan_fov = tan(glm::radians(fov/2));
    float len = glm::length(ref_p - eye);
//    aspect = width/height;

//    V = up*len*tan_fov;
//    H = right*len*aspect*tan_fov;

    //foward vec: ref_p - eye
    Vector3f foward = glm::normalize(ref_p - eye);
    //right : cross
    Vector3f right = glm::normalize(glm::cross(foward, world_up));
    //camera up
    Vector3f up = glm::cross(right, foward);

    //calculate matrix
    vp_mat = glm::perspective(fov, width / (float)height, near, far) * glm::lookAt(eye, ref_p, up);

    //calculate the inverse
    inv_vp_mat = glm::inverse(vp_mat);

    //set up variable
    e = eye;
    ref = ref_p;
    w = width;
    h = height;
    near_clip = near;
    far_clip = far;
}

//use a pixel coordinate to calculate a ray
//Yue Li helps me on this part
Ray Camera::ray_cast(int x_cord, int y_cord) {
    //transform to ndc
    float x = x_cord/(float)w * 2.f - 1.f;
    float y = 1.f - y_cord/(float)h * 2.f;

    glm::vec4 p(x,y,1.f,1.f); //pixel on film
    //transform to world space
    p = inv_vp_mat * (p * far_clip);
    glm::vec3 p3(p.x,p.y,p.z);
    return Ray(e,glm::normalize(p3 - e));
}
