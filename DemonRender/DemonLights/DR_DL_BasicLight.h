//
// Created by simon on 6/15/22.
//

#ifndef DEMONENGINE_DR_DL_BASICLIGHT_H
#define DEMONENGINE_DR_DL_BASICLIGHT_H

#include "../DR_Shader.h"
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <math.h>
#include <fmt/format.h>
//uniform vec3 lightPos;

namespace DemonRender {
    namespace DemonLight {

        typedef struct{
            float ambientStrength;
            glm::vec3 position;
            glm::vec3 direction;
            float cutOffDegree;
            float outerCutOffDegree;
            float specularStrength;
            unsigned int specularAccuracy;
            glm::vec3 colour;
            float distance;
        } DR_DL_BasicLight_CREATE_INFO;

        class DR_DL_BasicLight {
        public:
            static unsigned int lightCountLocation;
            static unsigned int lightCount;

            DR_DL_BasicLight(DR_Shader *targetShader) : _targetLightShader(targetShader) { lightNum = lightCount; lightCount += 1; }

            void initLight(DR_DL_BasicLight_CREATE_INFO creationInfo);

            DR_DL_BasicLight_CREATE_INFO* getLightInfo() { return &_lightInfo; }

            void renderLight();

        private:
            DR_Shader *_targetLightShader;
            DR_DL_BasicLight_CREATE_INFO _lightInfo;

            // shader locations
            unsigned int lightNum;
            unsigned int lightAmbientLocation;
            unsigned int lightPosLocation;
            unsigned int lightDirLocation;
            unsigned int lightCutoffLocation;
            unsigned int lightOuterCutOffLocation;
            unsigned int lightSpecStrLoc;
            unsigned int lightSpecValueLoc;
            unsigned int lightDistanceLoc;
            unsigned int lightColourLoc;

        };

    } // DemonRender
} // DemonLight

#endif //DEMONENGINE_DR_DL_BASICLIGHT_H
