//
// Created by simon on 6/1/22.
//

#ifndef DEMONENGINE_DW_TRANSFORM_H
#define DEMONENGINE_DW_TRANSFORM_H
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#ifndef NOPHYSX
#include <PhysX/foundation/PxVec3.h>
#endif

namespace DemonWorld {
    class DW_Transform {
    public:
        DW_Transform() {}

        void createTransform(glm::vec3 startPos = glm::vec3(0.0f),
                     glm::vec3 startRot = glm::vec3(0.0f),
                     glm::vec3 startScale = glm::vec3(1.0f)){
            position = startPos;
            rotation = startRot;
            _scale = startScale;
        }
        //virtual ~DW_Transform();

        void setPosition(glm::vec3 newPos) { position = newPos; }
        void setRotation(glm::vec3 newRot) { rotation = newRot; }
        void setRotation(glm::quat newRot) { rotation = newRot; }
        void setScale(glm::vec3 newScale) { _scale = newScale;}

        void translate(glm::vec3 offset) { position += offset; }
        void rotate(glm::quat offset) { rotation += offset; }
        void scale(glm::vec3 offset) { _scale += offset; }

        glm::vec3 getPosition() { return position; }
        glm::quat getRotation() { return rotation; }
        glm::vec3 getScale() { return _scale; }

        glm::mat4 getModel() {
            currentModel = glm::mat4(1.0f);
            currentModel = glm::translate(currentModel, position);
            /*
            currentModel = glm::rotate(currentModel, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
            currentModel = glm::rotate(currentModel, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
            currentModel = glm::rotate(currentModel, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
             */
            if (rotation != glm::quat(0.0f, 0.0f, 0.0f, 0.0f)){
                currentModel = glm::rotate(currentModel, glm::angle(rotation), glm::axis(rotation));
            }
            currentModel = glm::scale(currentModel, _scale);
            return currentModel;
        }

#ifndef NOPHYSX
        static physx::PxVec3 glmToPhys(glm::vec3 target){
            return physx::PxVec3(target.x, target.y, target.z);
        }

        static glm::vec3 PhysToGlm(physx::PxVec3 target){
            return glm::vec3(target.x, target.y, target.z);
        }
#endif

    protected:
        glm::vec3 position = glm::vec3(0.0f);
        glm::quat rotation = glm::quat(0.0f, 0.0f, 0.0f, 0.0f);
        glm::vec3 _scale = glm::vec3(1.0f);

        glm::mat4 currentModel = glm::mat4(1.0f);

    };

} // DemonWorld

#endif //DEMONENGINE_DW_TRANSFORM_H
