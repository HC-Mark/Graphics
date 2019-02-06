#include "ray.h"

void Ray::trans_ray(const glm::mat4 &trans_mat) {
    glm::vec4 temp_o = glm::vec4(origin,1);
    glm::vec4 temp_d = glm::vec4(direct,0); //direction doesn't need to be translated

    temp_o = trans_mat * temp_o;
    temp_d = trans_mat * temp_d;

    //transform back from v4 to v3
    origin = glm::vec3(temp_o.x, temp_o.y, temp_o.z);
    direct = glm::vec3(temp_d.x, temp_d.y, temp_d.z);
}
