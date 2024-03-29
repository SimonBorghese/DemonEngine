//
// Created by simon on 6/6/22.
//

#ifndef DEMONENGINE_DI_SCENELOADER_H
#define DEMONENGINE_DI_SCENELOADER_H

#include <DemonBase/b_Mesh.h>
#include <assimp/IOSystem.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/vector3.h>
#include <vector>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/printf.h>
#include <DFileSystem/FileSystem.h>

namespace DemonIO {

    class DI_SceneLoader {
    public:
        DI_SceneLoader();

        virtual ~DI_SceneLoader();

        static DemonBase::b_Mesh **loadMeshesFromFile(const char *fileName, unsigned int *meshCountOut, glm::vec3 scale = glm::vec3(1.0f));

        static DemonBase::b_Mesh **normalizeAroundOrigin(DemonBase::b_Mesh **meshes, uint32_t inLength, glm::vec3 *outOrigin);

    private:

    };

} // DemonIO

#endif //DEMONENGINE_DI_SCENELOADER_H
