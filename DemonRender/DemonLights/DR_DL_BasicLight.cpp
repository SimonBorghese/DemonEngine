//
// Created by simon on 6/15/22.
//

#include "DR_DL_BasicLight.h"

unsigned int DemonRender::DemonLight::DR_DL_BasicLight::lightCountLocation;
unsigned int DemonRender::DemonLight::DR_DL_BasicLight::lightCount;

namespace DemonRender {
    namespace DemonLight {
        void DR_DL_BasicLight::initLight(DR_DL_BasicLight_CREATE_INFO creationInfo) {
            lightAmbientLocation = _targetLightShader->getUniformLocation(fmt::format("Lights[{}].ambient", lightNum ).c_str());
            lightPosLocation = _targetLightShader->getUniformLocation(fmt::format("Lights[{}].position", lightNum ).c_str());
            lightDirLocation = _targetLightShader->getUniformLocation(fmt::format("Lights[{}].direction", lightNum ).c_str());
            lightCutoffLocation = _targetLightShader->getUniformLocation(fmt::format("Lights[{}].cutOff", lightNum ).c_str());
            lightOuterCutOffLocation = _targetLightShader->getUniformLocation(fmt::format("Lights[{}].outerCutOff", lightNum ).c_str());
            lightSpecStrLoc = _targetLightShader->getUniformLocation(fmt::format("Lights[{}].specularStrength", lightNum ).c_str());
            lightSpecValueLoc = _targetLightShader->getUniformLocation(fmt::format("Lights[{}].specValue", lightNum ).c_str());
            lightColourLoc = _targetLightShader->getUniformLocation(fmt::format("Lights[{}].colour", lightNum ).c_str());
            lightDistanceLoc = _targetLightShader->getUniformLocation(fmt::format("Lights[{}].distance", lightNum ).c_str());

            _lightInfo = creationInfo;
            if (lightCountLocation == 0) {
                lightCountLocation = _targetLightShader->getUniformLocation("LightCount");
            }
        }

        void DR_DL_BasicLight::renderLight() {
            _targetLightShader->bindFloat(lightAmbientLocation,  _lightInfo.ambientStrength);
            //_targetLightShader->bindVector3(lightPosLocation, glm::vec3(sin(SDL_GetTicks()) * 10, cos(SDL_GetTicks()) * 10, sin(SDL_GetTicks()) * 10));
            if (_lightInfo.position != glm::vec3(0.0f)) {
                _targetLightShader->bindVector3(lightPosLocation, _lightInfo.position);
            }
            else{
                _targetLightShader->bindVector3(lightPosLocation, glm::vec3(0.0f));
            }

            if (_lightInfo.direction != glm::vec3(0.0f)) {
                _targetLightShader->bindVector3(lightDirLocation, _lightInfo.direction);
            }
            else{
                _targetLightShader->bindVector3(lightDirLocation, glm::vec3(0.0f));
            }

            if (_lightInfo.cutOffDegree != 0.0f) {
                _targetLightShader->bindFloat(lightCutoffLocation, glm::radians(_lightInfo.cutOffDegree));
                _targetLightShader->bindFloat(lightOuterCutOffLocation, glm::radians(_lightInfo.outerCutOffDegree));
            }
            else{
                _targetLightShader->bindFloat(lightCutoffLocation, 0.0f);
                _targetLightShader->bindFloat(lightOuterCutOffLocation, 0.0f);
            }
            _targetLightShader->bindInt(lightSpecValueLoc, _lightInfo.specularAccuracy);
            _targetLightShader->bindFloat(lightSpecStrLoc, _lightInfo.specularStrength);
            _targetLightShader->bindInt(lightCountLocation, lightCount);
            _targetLightShader->bindVector3(lightColourLoc, _lightInfo.colour);
            _targetLightShader->bindFloat(lightDistanceLoc, _lightInfo.distance);
        }
    } // DemonRender
} // DemonLight