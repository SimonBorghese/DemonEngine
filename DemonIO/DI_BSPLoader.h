//
// Created by simon on 7/7/22.
//

#ifndef DEMONENGINE_DI_BSPLOADER_H
#define DEMONENGINE_DI_BSPLOADER_H
#include <DemonBase/b_Mesh.h>
#include <CBSP/CBSP.h>
#include <fmt/format.h>
#include <map>

namespace DemonIO {

    class DI_BSPLoader {
    public:
        static DemonBase::b_Mesh **loadMeshesFromFile(const char *fileName, unsigned int *meshCountOut, glm::vec3 scale = glm::vec3(1.0f));

        static DemonBase::b_Mesh **loadFacesFromCBSP(CBSP *map, unsigned int *meshCountOut, glm::vec3 scale = glm::vec3(1.0f));

        static DemonBase::b_Mesh **loadMeshesFromCBSP(CBSP *map, unsigned int *meshCountOut, glm::vec3 scale = glm::vec3(1.0f));
        static CBSP *createBSP(const char *filename);

        static glm::vec3 getPosFromEntity(CBSP_Entity *entity);
    private:

    };

} // DemonIO

#endif //DEMONENGINE_DI_BSPLOADER_H
