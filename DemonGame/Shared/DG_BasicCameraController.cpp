//
// Created by simon on 6/1/22.
//

#include "DG_BasicCameraController.h"

namespace DemonGame {
    void DG_BasicCameraController::updateCamera(){
        xRotate += eventHandler->getMouseXOffset() * camSensitivity;
        yRotate -= eventHandler->getMouseYOffset() * camSensitivity;
        yRotate = glm::clamp(yRotate, -89.0f, 89.0f);

        targetCamera->setEularAngles(xRotate, yRotate);


        if (eventHandler->getKey(SDL_SCANCODE_W)){
            targetCamera->setPosition((targetCamera->getPosition() + (targetCamera->getCameraFront() * camSpeed)));
        }
        if (eventHandler->getKey(SDL_SCANCODE_S)){
            targetCamera->setPosition((targetCamera->getPosition() - (targetCamera->getCameraFront() * camSpeed)));
        }
        if (eventHandler->getKey(SDL_SCANCODE_A)){
            targetCamera->setPosition(targetCamera->getPosition() - (glm::normalize(glm::cross(targetCamera->getCameraFront(), targetCamera->getCameraUp())) * camSpeed));
        }
        if (eventHandler->getKey(SDL_SCANCODE_D)){
            targetCamera->setPosition(targetCamera->getPosition() + (glm::normalize(glm::cross(targetCamera->getCameraFront(), targetCamera->getCameraUp())) * camSpeed));
        }
    }
} // DemonGame