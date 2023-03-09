//
// Created by simon on 6/1/22.
//

#include "DG_Entity.h"

namespace DemonGame {

    void DG_Entity::createEntityFromMesh(const char *meshFile,
                                         glm::vec3 pos,
                                         glm::vec3 rotation,
                                         glm::vec3 scale) {

        _loadedMesh = DemonIO::DI_SceneLoader::loadMeshesFromFile(
                DFS::FileSystem::getFS()->getModelPath(meshFile).c_str(),
                &_numMeshes,
                scale);

        _primaryMesh = new DGL::MeshRenderer(_shader, _loadedMesh, _numMeshes);
        mainTransform = _primaryMesh->getTransform();
        mainTransform->createTransform(pos, rotation, glm::vec3(1.0f));
    }

    void DG_Entity::createEntityFromExistingMesh(DemonBase::b_Mesh **meshes, uint32_t numMesh,
                                      glm::vec3 pos,
                                      glm::vec3 rotation){
        _loadedMesh = meshes;
        _numMeshes = numMesh;
        _primaryMesh = new DGL::MeshRenderer(_shader, _loadedMesh, _numMeshes);
        mainTransform = _primaryMesh->getTransform();
        mainTransform->createTransform(pos, rotation, glm::vec3(1.0f));
    }


    void DG_Entity::destroyEntity() {

        _primaryMesh->destroyMeshes();
        //delete mainTransform;
        delete _primaryMesh;
    }
} // DemonGame