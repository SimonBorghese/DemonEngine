//
// Created by simon on 6/6/22.
//

#include "DI_SceneLoader.h"

namespace DemonIO {
    DemonBase::b_Mesh **DI_SceneLoader::loadMeshesFromFile(const char *fileName, unsigned int *meshCountOut, glm::vec3 scale) {

        std::vector<DemonBase::b_Mesh> newMeshes;

        Assimp::Importer *oModelLoader = new Assimp::Importer;
        const aiScene *oScene = oModelLoader->ReadFile(fileName, aiProcess_Triangulate | aiProcess_GenUVCoords |
                                                                 aiProcess_FindInvalidData | aiProcess_OptimizeMeshes | aiProcess_JoinIdenticalVertices | aiProcess_FlipWindingOrder
                                                                                                                        | aiProcess_SortByPType | aiProcess_FlipUVs);
        //printf("Loading: %s got %d models\n", fileName, oScene->mNumMeshes);
        if (strcmp("", oModelLoader->GetErrorString()) != 0) {
            (fmt::print("Failed to load: {}, Error: {}\n", fileName, oModelLoader->GetErrorString()));
        }
        /*
        for (unsigned int a = 0; a < oScene->mNumAnimations; a++){
            aiAnimation *anim = oScene->mAnimations[a];
            printf("Reading anim: %s\n", anim->mName.C_Str());
            //oScene->mRootNode->FindNode(anim->mChannels[0]->mNodeName)->
            for (unsigned int c = 0; c < anim->mNumChannels; c++){
                aiNodeAnim *nodeAnim = anim->mChannels[c];

                //nodeAnim->
                for (unsigned int p = 0; p < nodeAnim->mNumPositionKeys; p++){
                    aiVector3D vector3D = nodeAnim->mPositionKeys[p].mValue;
                    printf("Found pos key: %f %f %f\n", vector3D.x, vector3D.y, vector3D.z);
                }
            }
        }
         */

        // Allocate our meshes
        DemonBase::b_Mesh **meshes = (DemonBase::b_Mesh **) malloc(sizeof(DemonBase::b_Mesh *) * oScene->mNumMeshes);

        for (unsigned int m = 0; m < oScene->mNumMeshes; m++) {
            unsigned int numVertices = oScene->mMeshes[m]->mNumVertices;
            // Stack allocations go brrrrr....
            Vertex m_vertices[numVertices];
            //oScene->mMeshes[0]->
            aiBone bone;
            //bone.
            for (unsigned int v = 0; v < numVertices; v++) {

                m_vertices[v].iBoneIds = glm::ivec4(-1);
                m_vertices[v].weights = glm::vec4(1.0f);
                m_vertices[v].animationBody = 0;

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
                        m_vertices[v].iTextCord.y = -(oScene->mMeshes[m]->mTextureCoords[t][v]).y;
                        break;
                    }
                    //printf("Got: %f %f in ASSIMP\n", m_vertices[v].iTextCord.x, m_vertices[v].iTextCord.y);
                }
                if (!oScene->mMeshes[m]->mTextureCoords[0]) {
                    m_vertices[v].iTextCord.x = 0.0f;
                    m_vertices[v].iTextCord.y = 0.0f;
                }
                m_vertices[v].iBoneIds = glm::ivec4(65535);
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
            meshes[m] = new DemonBase::b_Mesh(&m_vertices[0], numVertices, m_indices.data(), m_indices.size());

            meshes[m]->setModelName(&oScene->mMeshes[m]->mName);
            aiMaterial *matSafe = oScene->mMaterials[oScene->mMeshes[m]->mMaterialIndex];

            if (matSafe != NULL) {
                aiString tex_loc;
                matSafe->GetTexture(aiTextureType_DIFFUSE, 0, &tex_loc);
                meshes[m]->setTextureDiffuse(tex_loc);

                //matSafe->GetTexture(aiTextureType_NORMALS, 0, &tex_loc);
                //meshes[m]->setTextureDiffuse(tex_loc);
                //printf("Got Normals: %s\n", tex_loc.C_Str());
            }
            else{
                meshes[m]->setTextureDiffuse(aiString("data/missing.png"));
            }

            meshes[m]->setMaterial(matSafe);
        }

        // Allocate meshes
        *meshCountOut = oScene->mNumMeshes;

        delete oScene;

        return meshes;
    }

    DemonBase::b_Mesh **DI_SceneLoader::normalizeAroundOrigin(DemonBase::b_Mesh **meshes, uint32_t inLength, glm::vec3 *outOrigin){
        glm::vec3 origin = glm::vec3(0.0f);
        uint32_t totalVertices = 0;
        for (uint m = 0; m < inLength; m++){
            DemonBase::b_Mesh *mesh = meshes[m];
            for (uint v = 0; v < mesh->getVertexLength(); v++){
                origin += mesh->getVertices()[v].iPosition;
                totalVertices++;
            }
        }
        origin /= (float) totalVertices;
        *outOrigin = origin;
        //printf("Origin: %f %f %f\n", origin.x, origin.y, origin.z);

        DemonBase::b_Mesh **newMeshes = (DemonBase::b_Mesh **) malloc(sizeof(DemonBase::b_Mesh *) * inLength);
        for (unsigned int m = 0; m < inLength; m++) {
            DemonBase::b_Mesh *mesh = meshes[m];
            unsigned int numVertices = mesh->getVertexLength();
            // Stack allocations go brrrrr....
            Vertex m_vertices[numVertices];
            //oScene->mMeshes[0]->
            aiBone bone;
            //bone.
            //printf("NUM VERTICES: %d\n", numVertices);
            for (unsigned int v = 0; v < numVertices; v++) {

                m_vertices[v].iBoneIds = mesh->getVertices()[v].iBoneIds;
                m_vertices[v].weights = mesh->getVertices()[v].weights;
                m_vertices[v].animationBody = mesh->getVertices()[v].animationBody;


                memcpy(&m_vertices[v], &mesh->getVertices()[v], sizeof(Vertex));

                m_vertices[v].iPosition -= origin;
                //m_vertices->iPosition -= origin;

            }


            // Make it a heap allocation so we don't lose it
            newMeshes[m] = new DemonBase::b_Mesh(&m_vertices[0], numVertices, mesh->getIndices(), mesh->getIndexLength());

            newMeshes[m]->setModelName(mesh->getModelName());
            newMeshes[m]->setTextureDiffuse(mesh->getTextureDiffuse());
            newMeshes[m]->setMaterial(mesh->getMaterial());
        }
        return newMeshes;
    }
} // DemonIO