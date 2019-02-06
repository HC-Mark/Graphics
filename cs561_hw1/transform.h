#ifndef TRANSFORM_H
#define TRANSFORM_H
#include "globalincludes.h"

class Transform {
public:
    //when we change translate, we need to reform the matrix completely

    Transform() : translate(glm::vec3(0,0,0)), scale(glm::vec3(1,1,1)), rotate(glm::vec3(0,0,0)) { cal_mat();}
    Transform(glm::vec3 t, glm::vec3 r, glm::vec3 s) : translate(t), rotate(r), scale(s) { cal_mat();}

    //getter
    glm::mat4 get_worldTransform();
    glm::mat4 get_worldTransformInverse();
    glm::mat3 get_worldTransformInverseTranspose();

    //change transform data
    void change_translate(glm::vec3 t);
    void change_rotate(glm::vec3 r);
    void change_scale(glm::vec3 s);


private:
    glm::vec3 translate;
    glm::vec3 rotate; //each element store the Euler angle of rotation
    glm::vec3 scale;

    //glm matrix is column base
    //mat[column][row]
    glm::mat4 worldTransform;
    glm::mat4 worldTransformInverse;
    glm::mat3 worldTransformInverseTranspose;

    void cal_mat(); //update or calculate the three matrices
};


#endif // TRANSFORM_H
