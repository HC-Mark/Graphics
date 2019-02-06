#include "transform.h"

glm::mat4 Transform::get_worldTransform() {
    return worldTransform;
}
glm::mat4 Transform::get_worldTransformInverse() {
    return worldTransformInverse;
}
glm::mat3 Transform::get_worldTransformInverseTranspose() {
    return worldTransformInverseTranspose;
}

//update data

void Transform::change_translate(glm::vec3 t) {
    translate = t;
    cal_mat();
}
void Transform::change_rotate(glm::vec3 r) {
    rotate = r;
    cal_mat();
}
void Transform::change_scale(glm::vec3 s) {
    scale = s;
    cal_mat();
}

//calculate three matrices
void Transform::cal_mat() {
    glm::mat4 trans = glm::translate(glm::mat4(1.f),translate);
    glm::mat4 rot_x = glm::rotate(glm::mat4(1.f),rotate.x,glm::vec3(1,0,0));
    glm::mat4 rot_y = glm::rotate(glm::mat4(1.f),rotate.y,glm::vec3(0,1,0));
    glm::mat4 rot_z = glm::rotate(glm::mat4(1.f),rotate.z,glm::vec3(0,0,1));
    glm::mat4 scal = glm::scale(glm::mat4(1.f),scale);

    worldTransform = trans * rot_x * rot_y * rot_z * scal;
    worldTransformInverse = glm::inverse(worldTransform);
    worldTransformInverseTranspose = glm::mat3(1.f);
    glm::mat4 worldTransformInverseTranspose_raw = glm::transpose(worldTransformInverse);
    for(int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            worldTransformInverseTranspose[i][j] = worldTransformInverseTranspose_raw[i][j];
        }
    }


}
