#include "gl_helper.hpp"

//
#include "camera.hpp"
#include "geometry.hpp"
#include "trackball_ctrl.hpp"

int main() {
    int programID = initGLProgram("Water");

    if (programID == -1) {
        fprintf(stderr, "something went wrong while initializing");
        return programID;
    }

    PerspectiveCamera *camera = new PerspectiveCamera(70.0, 4.0 / 3.0, 0.1, 100.0);
    camera->setPosition(glm::vec3(0, 10, 10));
    //BoxGeometry *box = new BoxGeometry(1.0f, 1.0f, 1.0f);
    PlaneGeometry *plane = new PlaneGeometry(10.0f, 10.0f, 400, 400);
    TrackballControl *trackBallControl = new TrackballControl(window, camera);
    // TODO: implement scene graphs
    //render(camera, box);
    render(camera, plane);
}