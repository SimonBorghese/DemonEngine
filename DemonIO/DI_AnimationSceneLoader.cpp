//
// Created by simon on 8/16/22.
//

#include "DI_AnimationSceneLoader.h"

namespace DemonIO {
    DemonAnimation::DA_riggedMesh **DI_AnimationSceneLoader::loadMeshesFromFile(const char *fileName, unsigned int *meshCountOut, glm::vec3 scale, Assimp::Importer **importerOut) {
        currentBody++;
        assert(currentBody <= MAX_ANIMATED_BODIES);
        std::vector<DemonBase::b_Mesh> newMeshes;

        Assimp::Importer *oModelLoader = new Assimp::Importer;
        const aiScene *oScene = oModelLoader->ReadFile(fileName, aiProcess_Triangulate | aiProcess_GenUVCoords |
                                                                 aiProcess_FindInvalidData | aiProcess_OptimizeMeshes | aiProcess_JoinIdenticalVertices | aiProcess_LimitBoneWeights);
        if (strcmp("", oModelLoader->GetErrorString()) != 0) {
            (fmt::print("Failed to load: {}, Error: {}\n", fileName, oModelLoader->GetErrorString()));
        }

        // Allocate our meshes
        DemonAnimation::DA_riggedMesh **meshes = (DemonAnimation::DA_riggedMesh **) malloc(sizeof(DemonAnimation::DA_riggedMesh *) * oScene->mNumMeshes);

        unsigned int numBone = 0;
        int totalBone = 0;

        for (unsigned int m = 0; m < oScene->mNumMeshes; m++) {
            unsigned int numVertices = oScene->mMeshes[m]->mNumVertices;
            aiMesh *currentMesh = oScene->mMeshes[m];

            // Stack allocations go brrrrr....
            Vertex *m_vertices = (Vertex*) malloc(sizeof(Vertex) * (numVertices + 1));
            //oScene->mMeshes[0]->
            //bone.
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

                for (int t = 0; t < AI_MAX_NUMBER_OF_TEXTURECOORDS; t++) {
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
            std::map<std::string, DemonAnimation::BoneInfo*> *_boneMap = new std::map<std::string, DemonAnimation::BoneInfo*>;

            // Name:Animation
            // WE ALSO NEED TO FREE THIS AT SOME POINT
            std::map<unsigned int, aiAnimation*> *_animationMap = new std::map<unsigned int, aiAnimation*>;

            std::map<int, std::string> _boneNames;


            // Begin bone processing
            for (unsigned int b = 0; b < currentMesh->mNumBones; ++b){
                aiBone *currentBone = currentMesh->mBones[b];

                int boneID = -1;
                std::string boneName = currentBone->mName.C_Str();
                if (_boneMap->find(boneName) == _boneMap->end())
                {
                    DemonAnimation::BoneInfo *newBoneInfo = new DemonAnimation::BoneInfo;
                    newBoneInfo->id = numBone;
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

                for (int weightIndex = 0; weightIndex < currentBone->mNumWeights; weightIndex++)
                {
                    int vertexId = weights[weightIndex].mVertexId;
                    float weight = weights[weightIndex].mWeight;
                    
                    assert(vertexId <= numVertices);
                    for (unsigned int cb = 0; cb < MAX_BONE_INFLUENCE; cb++) {
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


            std::vector<uint32_t> m_indices;
            for (unsigned int f = 0; f < oScene->mMeshes[m]->mNumFaces; f++) {
                aiFace face = oScene->mMeshes[m]->mFaces[f];
                for (unsigned int i = 0; i < face.mNumIndices; i++) {
                    m_indices.push_back(face.mIndices[i]);
                }
            }
            /*
            Vertex m_REALvertices[m_indices.size()];
            for (unsigned int i = 0; i < m_indices.size(); i++){
                m_REALvertices[i] = m_vertices[m_indices.at(i)];
            }
             */
            // Make it a heap allocation so we don't lose it



            /*
             *
             *
             * REMINDER
             *
             *
             *
             * ADD MESHES INIT
             *
             *
             *
             *
             * HERE
                */

            auto globalTransformation = oScene->mRootNode->mTransformation;
            glm::mat4 globalMatrix = GetGLMMat(globalTransformation.Inverse());
            meshes[m] = new DemonAnimation::DA_riggedMesh(&m_vertices[0], numVertices,
                                                          m_indices.data(), m_indices.size(),
                                                          (_boneMap), (_animationMap),
                                                          globalMatrix, oScene->mRootNode,
                                                          currentBody);
            aiMaterial *matSafe = oScene->mMaterials[oScene->mMeshes[m]->mMaterialIndex];

            if (matSafe != NULL) {
                aiString tex_loc;
                matSafe->GetTexture(aiTextureType_DIFFUSE, 0, &tex_loc);
                meshes[m]->setTextureDiffuse(tex_loc);
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