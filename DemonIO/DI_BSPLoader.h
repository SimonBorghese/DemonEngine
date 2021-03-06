//
// Created by simon on 7/7/22.
//

#ifndef DEMONENGINE_DI_BSPLOADER_H
#define DEMONENGINE_DI_BSPLOADER_H
#include <DemonBase/b_Mesh.h>
#include <CBSP/CBSP.h>
#include <fmt/format.h>

namespace DemonIO {

    class DI_BSPLoader {
    public:
        static DemonBase::b_Mesh **loadMeshesFromFile(const char *fileName, unsigned int *meshCountOut, glm::vec3 scale = glm::vec3(1.0f));
    private:

    };

} // DemonIO

#endif //DEMONENGINE_DI_BSPLOADER_H
