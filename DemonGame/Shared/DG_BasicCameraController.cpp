//
// Created by simon on 6/1/22.
//

#include "DG_BasicCameraController.h"

namespace DemonGame {
    void DG_BasicCameraController::updateCamera() {
        xRotate += eventHandler->getMouseXOffset() * camSensitivity;
        yRotate -= eventHandler->getMouseYOffset() * camSensitivity;
        yRotate = glm::clamp(yRotate, -89.0f, 89.0f);
        if (xRotate >= 360.0f){
            xRotate = 0.0f;
        }
        if (xRotate < 0){
            xRotate = 360.0f + xRotate;
        }

        targetCamera->setEularAngles(xRotate, yRotate);

        if (targetController != nullptr) {
            glm::vec3 currentControllerPos = glm::vec3(0.0f);
            targetController->getPosition(&currentControllerPos);
            targetCamera->setPosition(currentControllerPos);
            /*

            if (eventHandler->getKey(SDL_SCANCODE_W)) {
                targetController->move((targetCamera->getFPSFront() * camSpeed));
            }
            if (eventHandler->getKey(SDL_SCANCODE_S)) {
                targetController->move( -(targetCamera->getFPSFront() * camSpeed));
            }
            if (eventHandler->getKey(SDL_SCANCODE_A)) {
                targetController->move(-(glm::normalize(
                        glm::cross(targetCamera->getFPSFront(), targetCamera->getCameraUp())) * camSpeed));
            }
            if (eventHandler->getKey(SDL_SCANCODE_D)) {
                targetController->move((glm::normalize(
                        glm::cross(targetCamera->getFPSFront(), targetCamera->getCameraUp())) * camSpeed));
            }
             */
        } else {
            /*
            if (eventHandler->getKey(SDL_SCANCODE_W)) {
                targetCamera->setPosition((targetCamera->getPosition() + (targetCamera->getCameraFront() * camSpeed)));
            }
            if (eventHandler->getKey(SDL_SCANCODE_S)) {
                targetCamera->setPosition((targetCamera->getPosition() - (targetCamera->getCameraFront() * camSpeed)));
            }
            if (eventHandler->getKey(SDL_SCANCODE_A)) {
                targetCamera->setPosition(targetCamera->getPosition() - (glm::normalize(
                        glm::cross(targetCamera->getCameraFront(), targetCamera->getCameraUp())) * camSpeed));
            }
            if (eventHandler->getKey(SDL_SCANCODE_D)) {
                targetCamera->setPosition(targetCamera->getPosition() + (glm::normalize(
                        glm::cross(targetCamera->getCameraFront(), targetCamera->getCameraUp())) * camSpeed));
            }
             */
        }
    }
} // DemonGame