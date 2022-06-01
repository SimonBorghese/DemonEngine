//
// Created by simon on 5/29/22.
//

#ifndef DEMONENGINE_DR_CAMERA_H
#define DEMONENGINE_DR_CAMERA_H
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>

#include <glad/glad.h>
#include "DR_Shader.h"

namespace DemonRender {

    class DR_Camera {
    public:
        DR_Camera(DR_Shader *tShader) : _targetShader(tShader) {}

        void configureProjection(float FOV, float twitterRatio, float zClose, float zFar){
            projectionMat = glm::perspective(glm::radians(FOV), twitterRatio, zClose, zFar);
            viewLocation = _targetShader->getUniformLocation("view");
            projectionLocation = _targetShader->getUniformLocation("projection");
        }

        void setPosition(glm::vec3 position) { cameraPos = position;    }
        void setPosition(float x, float y, float z) { cameraPos = glm::vec3(x,y,z); }

        void setEularAngles(glm::vec3 angles) { setEularAngles(angles.x, angles.y); }
        void setEularAngles(float x, float y) { cameraFront = glm::vec3(0.0f);
            cameraFront.x = cos(glm::radians(x)) * cos(glm::radians(y));
            cameraFront.y = sin(glm::radians(y));
            cameraFront.z = sin(glm::radians(x)) * cos(glm::radians(y));
        cameraFront = glm::normalize(cameraFront); }

        void setMatrix(){
            viewMat = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
            _targetShader->bindMatrix4f(viewLocation, viewMat);
            _targetShader->bindMatrix4f(projectionLocation, projectionMat);
        }

        glm::vec3 getCameraFront() { return cameraFront;}
        glm::vec3 getPosition() { return cameraPos; }

    private:
        DR_Shader *_targetShader;
        GLuint viewLocation = 0;
        GLuint projectionLocation = 0;
        glm::mat4 viewMat = glm::mat4(1.0f);
        glm::mat4 projectionMat = glm::mat4(1.0f);
        glm::vec3 cameraPos = glm::vec3(0.0f);
        glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    };

} // DemonRender

#endif //DEMONENGINE_DR_CAMERA_H
