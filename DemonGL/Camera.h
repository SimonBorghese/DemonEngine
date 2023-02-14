//
// Created by simon on 1/11/23.
//

#ifndef DEMONENGINE_CAMERA_H
#define DEMONENGINE_CAMERA_H
#include "Shader.h"
#include <DemonWorld/DW_Transform.h>

#undef UP_ANGLE
#define UP_ANGLE 5.0f

namespace DGL {

    class Camera {
    public:
        Camera() = default;

        void configureProjection(float FOV, float twitterRatio, float zClose, float zFar) {
            projectionMat = glm::perspective(glm::radians(FOV), twitterRatio, zClose, zFar);
            //projectionMat = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
            /*
            viewLocation = _targetShader->getUniformLocation("view");
            projectionLocation = _targetShader->getUniformLocation("projection");
            viewPosLocation = _targetShader->getUniformLocation("viewPos");
             */
        }

        void configureOrtho(float xRange, float yRange, float zClose, float zFar){
            projectionMat = glm::ortho(-xRange, xRange, -yRange, yRange, zClose, zFar);
        }


        void setPosition(glm::vec3 position) { cameraPos = position; }

        void setPosition(float x, float y, float z) { cameraPos = glm::vec3(x, y, z); }

        void setEularAngles(glm::vec3 angles) { setEularAngles(angles.x, angles.y); }

        void setEularAngles(float x, float y) {
            lastXE = x;
            lastYE = y;
            cameraFront = glm::vec3(0.0f);
            cameraFront.x = cos(glm::radians(x)) * cos(glm::radians(y));
            cameraFront.y = sin(glm::radians(y));
            cameraFront.z = sin(glm::radians(x)) * cos(glm::radians(y));
            cameraFront = glm::normalize(cameraFront);

            cameraFrontFPS = glm::vec3(0.0f);
            cameraFrontFPS.x = cos(glm::radians(x)) * cos(glm::radians(UP_ANGLE));
            cameraFrontFPS.y = sin(glm::radians(UP_ANGLE));
            cameraFrontFPS.z = sin(glm::radians(x)) * cos(glm::radians(UP_ANGLE));
            cameraFrontFPS = glm::normalize(cameraFrontFPS);

            cameraRight = glm::normalize(glm::cross(cameraUp, -cameraFront));
        }

        void regenerateFPS(int yMultiplier){
            cameraFrontFPS = glm::vec3(0.0f);
            cameraFrontFPS.x = cos(glm::radians(lastXE)) * cos(glm::radians(UP_ANGLE * (float) yMultiplier));
            cameraFrontFPS.y = sin(glm::radians(UP_ANGLE * (float) yMultiplier));
            cameraFrontFPS.z = sin(glm::radians(lastXE)) * cos(glm::radians(UP_ANGLE * (float) yMultiplier));
            cameraFrontFPS = glm::normalize(cameraFrontFPS);
        }

        void setMatrix() {
            viewMat = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            /*
            _targetShader->useProgram();
            _targetShader->bindMatrix4f(viewLocation, viewMat);
            _targetShader->bindMatrix4f(projectionLocation, projectionMat);
            _targetShader->bindVector3(viewPosLocation, cameraPos);
             */
            Shader::setView( viewMat);
            Shader::setProjection(projectionMat);
            Shader::_dynamicLights.viewPos = getPosition();

            Shader::remakeGlobals();
        }

        void setCameraFront(glm::vec3 newFront){
            cameraFront = newFront;

        }

        glm::vec3 getCameraFront() { return cameraFront; }

        glm::vec3 getFPSFront() { return cameraFrontFPS; }

        glm::vec3 getPosition() { return cameraPos; }

        glm::vec3 getCameraUp() { return cameraUp; }
        glm::vec3 getCameraRight() { return cameraRight; }

        glm::mat4 getView() { return viewMat; }

        glm::vec3 getForward() { regenerateFPS(1); return cameraFrontFPS;}
        glm::vec3 getBackward() { regenerateFPS(-1); return -cameraFrontFPS; }
        glm::vec3 getRight() { return (glm::normalize(
                    glm::cross(cameraFrontFPS, cameraUp)) * 2.0f); }
        glm::vec3 getLeft() { return -getRight(); }

        float getXRotation() const { return lastXE; }
        float getYRotation() const { return lastYE; }

        glm::mat4 getProjectionMat() {return projectionMat; }

    private:
        // Last X & Y Euler angle
        float lastXE = 0.0f, lastYE = 0.0f;
        glm::mat4 viewMat = glm::mat4(1.0f);
        glm::mat4 projectionMat = glm::mat4(1.0f);
        glm::vec3 cameraPos = glm::vec3(0.0f);
        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 cameraFrontFPS = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 cameraRight = glm::vec3(0.0f, 0.0f, 1.0f);
    };

} // DGL

#endif //DEMONENGINE_CAMERA_H
