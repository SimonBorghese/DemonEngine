//
// Created by simon on 6/1/22.
//

#include "DG_BasicCameraController.h"

namespace DemonGame {
    void DG_BasicCameraController::updateCamera(){
        xRotate += eventHandler->getMouseXOffset() * camSensitivity;
        yRotate -= eventHandler->getMouseYOffset() * camSensitivity;
        printf("Rotate: %f %f\n", xRotate, yRotate);
        yRotate = std::clamp(yRotate, -89.0f, 89.0f);
        targetCamera->setEularAngles(xRotate, yRotate);


        if (eventHandler->getKey(SDL_SCANCODE_W)){
            targetCamera->setPosition((targetCamera->getPosition() + (targetCamera->getCameraFront() * 1.0f)));
        }
        if (eventHandler->getKey(SDL_SCANCODE_S)){
            targetCamera->setPosition((targetCamera->getPosition() - (targetCamera->getCameraFront() * 1.0f)));
        }
    }
} // DemonGame