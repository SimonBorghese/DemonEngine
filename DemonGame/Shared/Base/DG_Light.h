//
// Created by simon on 6/23/22.
//

#ifndef DEMONENGINE_DG_LIGHT_H
#define DEMONENGINE_DG_LIGHT_H
#include <glm/glm.hpp>

namespace DemonGame {

    class DG_Light {
    public:
        virtual void createLight() = 0;
        virtual void renderLight() = 0;
        virtual void killLight() = 0;

        glm::vec3 getColor() { return _colour; }
        float getStrength() { return _strength; }

        void setColor(glm::vec3 newColor) { _colour = newColor; }
        void setStrength(float newStrength) {_strength = newStrength; }
    protected:
        glm::vec3 _colour;
        float _strength;


    };

} // DemonGame

#endif //DEMONENGINE_DG_LIGHT_H
