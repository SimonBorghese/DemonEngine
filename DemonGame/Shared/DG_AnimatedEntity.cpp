//
// Created by simon on 9/15/22.
//

#include "DG_AnimatedEntity.h"

namespace DemonGame {
    void DG_AnimatedEntity::createEntityFromMesh(const char *meshFile, glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale)
    {

        //DG_Entity::createEntityFromMesh(meshFile, pos, rotation, scale);
        //mainTransform.createTransform(pos, rotation, glm::vec3(1.0f));
        //mainTransform = new DemonWorld::DW_Transform(pos, rotation, scale);


        animMeshes = DemonIO::DI_AnimationSceneLoader::loadMeshesFromFile(
                DFS::FileSystem::getFS()->getModelPath(meshFile).c_str(),
                &numMeshes,
                scale);

        DGL::Mesh **_renderMesh = (DGL::Mesh **) malloc(sizeof(DGL::Mesh *) * numMeshes);
        for (uint m = 0; m < numMeshes; m++) {
            auto currentMesh = new DGL::Mesh(animMeshes[m]->getVerticesVector(), animMeshes[m]->getIndicesVector(),
                                             std::string(animMeshes[m]->getTextureDiffuse().C_Str()),
                                             std::string(animMeshes[m]->getTextureNormal().C_Str()));
            currentMesh->createMesh();
            _renderMesh[m] = currentMesh;
        }

        _primaryMesh = new DGL::MeshRenderer(_shader, _renderMesh, numMeshes);
        mainTransform = _primaryMesh->getTransform();
        mainTransform->createTransform(pos, rotation, glm::vec3(1.0f));

    }

    void DG_AnimatedEntity::playAnimation(float currentTime){
        for (unsigned int m = 0; m < numMeshes; m++){
            //animMeshes[m]->setShader(_shader);
            animMeshes[m]->doAnimation(currentTime);
        }
    }
    void DG_AnimatedEntity::setAnimation(int index){
        for (unsigned int m = 0; m < numMeshes; m++){
            animMeshes[m]->setAnimation(index);
        }
    }

    void DG_AnimatedEntity::setAnimation(std::string name){
        for (unsigned int m = 0; m < numMeshes; m++){
            animMeshes[m]->setAnimation(name);
        }
    }

    void DG_AnimatedEntity::playOnce(float currentTime){
        for (unsigned int m = 0; m < numMeshes; m++){
            //animMeshes[m]->setShader(_shader);
            animMeshes[m]->playOnce(currentTime);
        }
    }

    int DG_AnimatedEntity::isAnimationFinished(float currentTime){
        for (unsigned int m = 0; m < numMeshes; m++){
            if (!animMeshes[m]->animationFinished(currentTime)){
                return 0;
            }
        }
        return 1;
    }
    int DG_AnimatedEntity::getAnimationIndex(){
        return animMeshes[0]->getAnimation();
    }

    std::vector<DemonAnimation::DA_riggedMesh*> DG_AnimatedEntity::getMeshes(){
        return {animMeshes[0], animMeshes[numMeshes-1]};
    }

    void DG_AnimatedEntity::update(DGL::Shader *overrideShader){
        if (overrideShader) {
            for (unsigned int m = 0; m < numMeshes; m++){
                animMeshes[m]->setShader(overrideShader);
            }
        } else{
            for (unsigned int m = 0; m < numMeshes; m++){
                animMeshes[m]->setShader(_shader);
            }
        }
        if (_preRender) { _preRender(); }
        if (_updateFunc) {_updateFunc((DG_Entity*) this);}
        if (enableRender) {
            _primaryMesh->render(overrideShader);
        }
        if (_postRender) { _postRender();}
    }
} // DemonGame