//
// Created by simon on 10/3/22.
//

#ifndef DEMONENGINE_DI_HEIGHTMAP_H
#define DEMONENGINE_DI_HEIGHTMAP_H

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

#include <STB_IMAGE/stb_image.h>

namespace DemonIO {

    class DI_HeightMap {
    public:
        DI_HeightMap();

        virtual ~DI_HeightMap();

        static DemonBase::b_Mesh **loadMeshesFromFile(const char *fileName, unsigned int *meshCountOut, glm::vec3 scale = glm::vec3(1.0f));

    };

} // DemonIO

#endif //DEMONENGINE_DI_HEIGHTMAP_H
