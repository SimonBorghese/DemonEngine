//
// Created by simon on 8/16/22.
//

#include "DI_AnimationSceneLoader.h"

namespace DemonIO {
    DemonAnimation::DA_riggedMesh **DI_AnimationSceneLoader::loadMeshesFromFile(const char *fileName, unsigned int *meshCountOut, glm::vec3 scale, Assimp::Importer **importerOut) {
        currentBody++;
        assert(currentBody <= MAX_ANIMATED_BODIES);
        std::vector<DemonBase::b_Mesh> newMeshes;

        auto *oModelLoader = new Assimp::Importer;
        const aiScene *oScene = oModelLoader->ReadFile(fileName, aiProcess_Triangulate | aiProcess_GenUVCoords |
                                                                 aiProcess_FindInvalidData | aiProcess_OptimizeMeshes | aiProcess_JoinIdenticalVertices | aiProcess_LimitBoneWeights);
        if (strcmp("", oModelLoader->GetErrorString()) != 0) {
            (fmt::print("Failed to load: {}, Error: {}\n", fileName, oModelLoader->GetErrorString()));
        }

        // Allocate our meshes
        auto **meshes = (DemonAnimation::DA_riggedMesh **) malloc(sizeof(DemonAnimation::DA_riggedMesh*) * oScene->mNumMeshes);

        unsigned int numBone = 0;
        int totalBone = 0;

        for (unsigned int m = 0; m < oScene->mNumMeshes; m++) {

            unsigned int numVertices = oScene->mMeshes[m]->mNumVertices;
            aiMesh *currentMesh = oScene->mMeshes[m];

            auto *m_vertices = (Vertex*) malloc(sizeof(Vertex) * (numVertices + 1));


            for (unsigned int v = 0; v < numVertices; v++) {

                m_vertices[v].iBoneIds = glm::ivec4(-1);
                m_vertices[v].weights = glm::vec4(0.0f);
                m_vertices[v].animationBody = currentBody;

                m_vertices[v].iPosition.x = oScene->mMeshes[m]->mVertices[v].x * scale.x;
                m_vertices[v].iPosition.y = oScene->mMeshes[m]->mVertices[v].y * scale.y;
                m_vertices[v].iPosition.z = oScene->mMeshes[m]->mVertices[v].z * scale.z;

                if (oScene->mMeshes[m]->HasNormals()) {
                    m_vertices[v].iNormal.x = oScene->mMeshes[m]->mNormals[v].x * scale.x;
                    m_vertices[v].iNormal.y = oScene->mMeshes[m]->mNormals[v].y * scale.y;
                    m_vertices[v].iNormal.z = oScene->mMeshes[m]->mNormals[v].z * scale.z;
                } else {
                    m_vertices[v].iNormal.x = 0.0f;
                    m_vertices[v].iNormal.y = 0.0f;
                    m_vertices[v].iNormal.z = 0.0f;
                }

                for (unsigned int t = 0; t < oScene->mMeshes[m]->mNumUVComponents[0]; t++) {
                    if (oScene->mMeshes[m]->mTextureCoords[t]) {
                        m_vertices[v].iTextCord.x = (oScene->mMeshes[m]->mTextureCoords[t][v]).x;
                        m_vertices[v].iTextCord.y = (oScene->mMeshes[m]->mTextureCoords[t][v]).y;
                        break;
                    }
                }
                if (!oScene->mMeshes[m]->mTextureCoords[0]) {
                    m_vertices[v].iTextCord.x = 0.0f;
                    m_vertices[v].iTextCord.y = 0.0f;
                }
            }

            // Bone/Node Name:BoneID
            auto *_boneMap = new std::map<std::string, DemonAnimation::BoneInfo*>;

            // Name:Animation
            // WE ALSO NEED TO FREE THIS AT SOME POINT
            auto *_animationMap = new std::map<unsigned int, aiAnimation*>;

            std::map<int, std::string> _boneNames;


            // Begin bone processing
            for (unsigned int b = 0; b < currentMesh->mNumBones; ++b){
                aiBone *currentBone = currentMesh->mBones[b];

                int boneID;
                std::string boneName = currentBone->mName.C_Str();
                if (_boneMap->find(boneName) == _boneMap->end())
                {
                    auto *newBoneInfo = new DemonAnimation::BoneInfo;
                    newBoneInfo->id = (int) numBone;
                    newBoneInfo->offset = GetGLMMat(currentBone->mOffsetMatrix);
                    (*_boneMap)[boneName] = newBoneInfo;
                    boneID = totalBone;
                    _boneNames[totalBone] = boneName;
                    totalBone++;
                    numBone++;
                }
                else
                {
                    boneID = (*_boneMap)[boneName]->id;
                }
                assert(boneID != -1);
                auto weights = currentBone->mWeights;

                for (unsigned int weightIndex = 0; weightIndex < currentBone->mNumWeights; weightIndex++)
                {
                    unsigned int vertexId = weights[weightIndex].mVertexId;
                    float weight = weights[weightIndex].mWeight;
                    
                    assert(vertexId <= numVertices);
                    for (int cb = 0; cb < MAX_BONE_INFLUENCE; cb++) {
                        if (m_vertices[vertexId].weights[cb] == 0.0f) {
                            m_vertices[vertexId].weights[cb] = weight;
                            m_vertices[vertexId].iBoneIds[cb] = boneID;
                            break;
                        }
                    }
                }
            }


            for (unsigned int a = 0; a < oScene->mNumAnimations; a++){
                aiAnimation *currentAnim = oScene->mAnimations[a];
                _animationMap->insert(std::pair<unsigned int, aiAnimation*>(a, currentAnim));
                printf("Adding anim: %s %d\n", currentAnim->mName.C_Str(), a);
                //_animationMap[a] = oScene->mAnimations[5];
            }


            auto m_indices = new std::vector<uint32_t>;
            for (unsigned int f = 0; f < oScene->mMeshes[m]->mNumFaces; f++) {
                aiFace face = oScene->mMeshes[m]->mFaces[f];
                for (unsigned int i = 0; i < face.mNumIndices; i++) {
                    m_indices->push_back(face.mIndices[i]);
                }
            }

            auto globalTransformation = oScene->mRootNode->mTransformation;
            glm::mat4 globalMatrix = GetGLMMat(globalTransformation.Inverse());

            meshes[m] = new DemonAnimation::DA_riggedMesh(&m_vertices[0], numVertices,
                                                          m_indices->data(), m_indices->size(),
                                                          (_boneMap), (_animationMap),
                                                          globalMatrix, oScene->mRootNode,
                                                          currentBody);

            delete m_indices;
            free(m_vertices);

            aiMaterial *matSafe = oScene->mMaterials[oScene->mMeshes[m]->mMaterialIndex];

            if (matSafe != nullptr) {
                /*
                aiString tex_loc;
                matSafe->GetTexture(aiTextureType_DIFFUSE, 0, &tex_loc);
                meshes[m]->setTextureDiffuse(tex_loc);
                 */
                aiString tex_loc;
                std::string cFileName(fileName);
                std::string baseName;
                std::string textureName;

                baseName = cFileName.substr(0, cFileName.find_last_of("/\\"));
                baseName = baseName.substr(baseName.find_last_of("/\\") + 1);

                matSafe->GetTexture(aiTextureType_DIFFUSE, 0, &tex_loc);
                std::string texturePath = std::string(tex_loc.C_Str());
                textureName = texturePath.substr(texturePath.find_last_of("/\\") + 1);

                //printf("Got Path: %s\n", DFS::FileSystem::getFS()->getTexturePath(baseName, textureName).c_str());
                meshes[m]->setTextureDiffuse(
                        aiString(DFS::FileSystem::getFS()->getTexturePath(baseName, textureName).c_str()));
            }
            else{
                meshes[m]->setTextureDiffuse(aiString("data/missing.png"));
            }

            meshes[m]->setMaterial(matSafe);

        }

        // Allocate meshes
        *meshCountOut = oScene->mNumMeshes;
        // WE NEED TO FREE SCENE AT SOME POINT
        //delete oModelLoader;
        //delete oScene;
        if (importerOut){
            *importerOut = oModelLoader;
        }

        return meshes;
    }
} // DemonIO