#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GLFW/glfw3.h>

#include <cmath>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>


class Camera {
   protected:
    // vec3 for the viewMatrix
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    glm::vec3 front;
};

class PerspectiveCamera : public Camera {
   private:
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 modelMatrix;
    glm::mat4 transform;
    glm::mat4 modelViewMatrix;
    // floats for the projeciton matrix
    GLfloat aspectRatio;
    GLfloat fieldOfView;
    GLfloat near;
    GLfloat far;

   public:
    //constructor
    PerspectiveCamera(GLfloat _fov, GLfloat _aspectRatio, GLfloat _near, GLfloat _far);

    // setters
    void setPosition(glm::vec3 newPosition);
    void setTarget(glm::vec3 newTarget);

    void addTransformation(glm::mat4 transform);

    // getters
    glm::vec3 getPosition();
    glm::mat4 getProjectionMatrix();
    glm::mat4 getViewMatrix();
    glm::mat4 getModelViewMat();

    // gets the inverse transpose of the model view matrix for normal transformation
    glm::mat4 inverseTranspose();
};
#endif