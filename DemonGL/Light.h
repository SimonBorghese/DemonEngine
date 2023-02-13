//
// Created by simon on 1/12/23.
//

#ifndef DEMONENGINE_LIGHT_H
#define DEMONENGINE_LIGHT_H
#include "Shader.h"
#include <glm/glm.hpp>
#include <vector>
#include <functional>
#include <fmt/format.h>


/*
 *
 * struct _lightSpec{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float distance;

    vec3 colour;
    float ambient;
    float specularStrength;
    int  specValue;
};

 */

namespace DGL {
    class Light {
    public:
        Light();

        void createShadowBuffer(uint32_t width, uint32_t height);
        void renderShadowBuffer(DGL::Shader *shadowShader, std::function<void()> renderFunction);

        glm::vec3 getPosition();
        glm::vec3 getDirection();
        float getCutoff();
        float getOuterCutoff();
        float getDistance();
        glm::vec3 getColour();
        float getAmbient();
        float getSpecularStrength();
        int getSpecularValue();

        void setPosition(glm::vec3 newPos);
        void setDirection(glm::vec3 newDir);
        void setCutoff(float newCut);
        void setOuterCutoff(float newOutCut);
        void setDistance(float newDist);
        void setColor(glm::vec3 newColor);
        void setAmbient(float newAmbient);
        void setSpecularStrength(float newStrength);
        void setSpecularValue(int newSpec);

        _lightSpec* getConfig();

        void setShadowResolution(uint32_t width, uint32_t height);

    private:
        GLuint _lightNumber;
        GLuint _shadowBuffer = (GLuint) -1;
        GLuint _shadowTexture = (GLuint) -1;
        uint32_t _shadowWidth = 0, _shadowHeight = 0;
        std::vector<glm::mat4> shadowTransforms;
        _lightSpec _lightConfig;

    };

} // DGL

#endif //DEMONENGINE_LIGHT_H
