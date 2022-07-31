//
// Created by simon on 6/1/22.
//

#ifndef DEMONENGINE_DG_BASICCAMERACONTROLLER_H
#define DEMONENGINE_DG_BASICCAMERACONTROLLER_H

#include "DG_Event.h"
#include <DemonRender/DR_Camera.h>
#include <DemonPhysics/DP_CharacterController.h>
#include <iostream>
#include <algorithm>

namespace DemonGame {
    class DG_BasicCameraController {
    public:
        DG_BasicCameraController(DG_Event *targetEvent, DemonRender::DR_Camera *camera, float sensitivity = 0.1f,
                                 float speed = 1.0f) :
                eventHandler(targetEvent),
                targetCamera(camera),
                camSensitivity(sensitivity),
                camSpeed(speed) {}

        void setSensitivity(float newSens) { camSensitivity = newSens; }

        void setSpeed(float newSpeed) { camSpeed = newSpeed; }

        void setController(DemonPhysics::DP_CharacterController *target) { targetController = target; }

        void updateCamera();

        void setRotation(glm::vec2 rotation) { xRotate = rotation.x; yRotate = rotation.y;}
        glm::vec2 getRotation() { return glm::vec2(xRotate, yRotate); }

    private:
        DG_Event *eventHandler;
        DemonRender::DR_Camera *targetCamera;
        DemonPhysics::DP_CharacterController *targetController = nullptr;
        float camSensitivity = 0.0f;
        float camSpeed = 0.0f;
        float xRotate = 0.0f;
        float yRotate = 0.0f;

    };

} // DemonGame

#endif //DEMONENGINE_DG_BASICCAMERACONTROLLER_H
