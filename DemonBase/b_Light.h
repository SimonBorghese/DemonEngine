//
// Created by simon on 6/27/22.
//

#ifndef DEMONENGINE_B_LIGHT_H
#define DEMONENGINE_B_LIGHT_H
#include <glm/vec3.hpp>
#include <DemonRender/DR_Shader.h>

namespace DemonBase {

    class b_Light {
    public:
        virtual void renderLight() = 0;

    protected:
        glm::vec3 _colour = glm::vec3(1.0f);
        float _ambientStrength = 1.0f;
        float _specularStrength = 1.0f;
        float _specularAccuracy = 32;

        void *_prevShader;
        unsigned int _colourLoc = 0;
        unsigned int _ambiStrLoc = 0;
        unsigned int _specStrLoc = 0;
        unsigned int _specAccLoc = 0;



    };

} // DemonBase

#endif //DEMONENGINE_B_LIGHT_H
