#ifndef TRACKBALLCTRL_HPP
#define TRACKBALLCTRL_HPP

#include <cmath>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

#include "camera.hpp"
#include "utilities.hpp"

// uses the shoemake's algorithm to do the trackball
// the algorithm puts a virtual hemiphere over the 2D screen, called the arcball
// rotation is based on mouse movements over the arc ball

class TrackballControl {
   private:
    GLFWwindow* window;
    //PerspectiveCamera* camera;
    glm::vec3 currCoord;

    // solution for accessing members of the control class from within the glfw callback
    inline static auto cursorPositionCallback(GLFWwindow* window, double xPos, double yPos) -> void {
        int windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        if (xPos >= windowWidth || yPos >= windowHeight) return;

        float xPosNorm, yPosNorm;
        normalizeCoord(windowWidth, windowHeight, xPos, yPos, xPosNorm, yPosNorm);
        TrackballControl* trackballControl = static_cast<TrackballControl*>(glfwGetWindowUserPointer(window));
        // check if the left mouse button is pressed
        int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
        if (state == GLFW_PRESS) {
            // calculate the axis and angle of rotation between 2 coords of the mouse
            glm::vec3 nextCoord = sphericalProjection(xPosNorm, yPosNorm);

            std::cout << "curr coord:" << glm::to_string(trackballControl->getCurrCoord()) << std::endl;
            std::cout << "next coord:" << glm::to_string(nextCoord) << std::endl;

            // // don't do anything if the cooridnate has not changed
            // if (nextCoord == trackballControl->getCurrCoord()) return;

            float angle = angleBetween(trackballControl->getCurrCoord(), nextCoord);
            glm::vec3 cross = glm::cross(trackballControl->getCurrCoord(), nextCoord);

            cross = cross / glm::length(cross);  // normalize the cross product, this step is important

            std::cout << "angle:" << angle << std::endl;
            std::cout << "cross:" << glm::to_string(cross) << std::endl;

            if (std::isnan(angle) || angle < 0.001) return;

            // calculate the rotation quaternion from the angle and axis (glm requires angles in degrees)
            glm::quat rotQuat = glm::quat(glm::cos(angle / 2), glm::sin(angle / 2) * cross);

            // get rotational matrxi from quaternion
            glm::mat4 rotMatrix = glm::toMat4(rotQuat);

            std::cout << "rotational matrix:" << glm::to_string(rotMatrix) << std::endl;
            std::cout << "===============================================" << std::endl;

            // add the rotation transformation to the camera
            trackballControl->addCameraTransform(rotMatrix);

            // update the current mouse position
            trackballControl->setCurrMouseCoords(nextCoord);
        }
    };

    //callback function for mouse button events
    inline static auto mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) -> void {
        TrackballControl* trackballControl = static_cast<TrackballControl*>(glfwGetWindowUserPointer(window));

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            // get cursor position
            double xPos, yPos;
            glfwGetCursorPos(window, &xPos, &yPos);
            float xPosNorm, yPosNorm;

            // get window size and normalize coordinate
            int windowWidth, windowHeight;
            glfwGetWindowSize(window, &windowWidth, &windowHeight);
            normalizeCoord(windowWidth, windowHeight, xPos, yPos, xPosNorm, yPosNorm);

            // project the 2D coordinate on to the 3D arcball
            trackballControl->setCurrMouseCoords(sphericalProjection(xPosNorm, yPosNorm));
        }
    }

    // project 2D x and y onto a unit sphere(trackball)
    // TODO: potentially make dynamic trackball size
    static glm::vec3 sphericalProjection(float x, float y);

    // normalize the coordinate from screen to [-1, 1]
    static void normalizeCoord(int windowWidth, int windowHeight, float xPos, float yPos, float& xPosNorm, float& yPosNorm);

   public:
    float currMouseX;
    float currMouseY;
    float newMouseX;
    float newMouseY;
    PerspectiveCamera* camera;

    TrackballControl(GLFWwindow* _window, PerspectiveCamera* _camera);
    // setters
    void setCurrMouseCoords(glm::vec3 sphereCoord);

    // getters
    glm::vec3 getCurrCoord();

    void addCameraTransform(glm::mat4 transform);
};

#endif