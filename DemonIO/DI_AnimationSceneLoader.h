//
// Created by simon on 8/16/22.
//

#ifndef DEMONENGINE_DI_ANIMATIONSCENELOADER_H
#define DEMONENGINE_DI_ANIMATIONSCENELOADER_H

#include <DemonBase/b_Mesh.h>
#include <DemonAnimation/DA_riggedMesh.h>
#include <assimp/IOSystem.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/vector3.h>
#include <vector>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/printf.h>

#include <glm/glm.hpp>

#include <glm/ext/quaternion_float.hpp>

#define MAX_BONE_INFLUENCE 4
#define MAX_ANIMATED_BODIES 10

namespace DemonIO {
    class DI_AnimationSceneLoader {
    public:
        DI_AnimationSceneLoader();

        virtual ~DI_AnimationSceneLoader();

        static DemonAnimation::DA_riggedMesh **loadMeshesFromFile(const char *fileName, unsigned int *meshCountOut, glm::vec3 scale = glm::vec3(1.0f), Assimp::Importer **importerOut = nullptr);
        static inline int currentBody = -1;


    private:

    };

} // DemonIO

#endif //DEMONENGINE_DI_ANIMATIONSCENELOADER_H
