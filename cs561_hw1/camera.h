#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"

class Camera {
public:
    Point3f e;
    Point3f ref;
    int w;
    int h;
    float near_clip;
    float far_clip;
    glm::mat4 vp_mat;
    glm::mat4 inv_vp_mat;
    Camera(Point3f eye, Point3f ref_p,Vector3f world_up, float fov, float far, float near, int width, int height);

    //use a pixel coordinate to calculate a ray
    Ray ray_cast(int x_cord, int y_cord);

};

#endif // CAMERA_H
