//
// Created by simon on 5/29/22.
//

#ifndef DEMONENGINE_DR_CAMERA_H
#define DEMONENGINE_DR_CAMERA_H

#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#define UP_ANGLE 10.0f

#include <glm/ext.hpp>

#include <glad/glad.h>
#include "DR_Shader.h"

namespace DemonRender {

    class DR_Camera {
    public:
        DR_Camera(DR_Shader *tShader) : _targetShader(tShader) {}

        void configureProjection(float FOV, float twitterRatio, float zClose, float zFar) {
            projectionMat = glm::perspective(glm::radians(FOV), twitterRatio, zClose, zFar);
            //projectionMat = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
            viewLocation = _targetShader->getUniformLocation("view");
            projectionLocation = _targetShader->getUniformLocation("projection");
            viewPosLocation = _targetShader->getUniformLocation("viewPos");
        }

        void setShader(DR_Shader *shader){

            shader->useProgram();
            if (shader != _targetShader){
                viewLocation = shader->getUniformLocation("view");
                projectionLocation = shader->getUniformLocation("projection");
                viewPosLocation = shader->getUniformLocation("viewPos");
            }
            _targetShader = shader;
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
            cameraFrontFPS.x = cos(glm::radians(lastXE)) * cos(glm::radians(UP_ANGLE * yMultiplier));
            cameraFrontFPS.y = sin(glm::radians(UP_ANGLE * yMultiplier));
            cameraFrontFPS.z = sin(glm::radians(lastXE)) * cos(glm::radians(UP_ANGLE * yMultiplier));
            cameraFrontFPS = glm::normalize(cameraFrontFPS);
        }

        void setMatrix() {
            viewMat = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            _targetShader->bindMatrix4f(viewLocation, viewMat);
            _targetShader->bindMatrix4f(projectionLocation, projectionMat);
            _targetShader->bindVector3(viewPosLocation, cameraPos);
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

        float getXRotation() { return lastXE; }
        float getYRotation() { return lastYE; }

    private:
        DR_Shader *_targetShader;
        GLuint viewLocation = 0;
        GLuint projectionLocation = 0;
        GLuint viewPosLocation = 0;
        // Last X & Y Euler angle
        float lastXE, lastYE;
        glm::mat4 viewMat = glm::mat4(1.0f);
        glm::mat4 projectionMat = glm::mat4(1.0f);
        glm::vec3 cameraPos = glm::vec3(0.0f);
        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 cameraFrontFPS = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 cameraRight = glm::vec3(0.0f, 0.0f, 1.0f);
    };

} // DemonRender

#endif //DEMONENGINE_DR_CAMERA_H
