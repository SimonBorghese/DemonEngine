//
// Created by simon on 7/11/22.
//

#ifndef DEMONENGINE_B_MESHENTITY_H
#define DEMONENGINE_B_MESHENTITY_H

#include "b_GameObject.h"
#include <glm/vec3.hpp>

namespace DemonBase {

    class b_MeshEntity : public b_GameObject{
    public:
        virtual void createEntityFromMesh(const char *meshFile,
                                  glm::vec3 pos = glm::vec3(0.0f),
                                  glm::vec3 rotation = glm::vec3(0.0f),
                                  glm::vec3 scale = glm::vec3(1.0f)) = 0;

        virtual void destroyEntity() = 0;

        int pointer;
    private:

    };

} // DemonBase

#endif //DEMONENGINE_B_MESHENTITY_H
