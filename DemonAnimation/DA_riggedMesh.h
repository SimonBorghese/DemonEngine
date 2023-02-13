//
// Created by simon on 8/16/22.
//

#ifndef DEMONENGINE_DA_RIGGEDMESH_H
#define DEMONENGINE_DA_RIGGEDMESH_H
#include <DemonBase/b_Mesh.h>

#include <assimp/IOSystem.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/vector3.h>

#include <map>
#include <string>
#include <fmt/format.h>
#include <vector>
#include <DemonGL/DemonGL.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

static inline glm::mat4 GetGLMMat(const aiMatrix4x4& aiMat){
    return {
            aiMat.a1, aiMat.b1, aiMat.c1, aiMat.d1,
            aiMat.a2, aiMat.b2, aiMat.c2, aiMat.d2,
            aiMat.a3, aiMat.b3, aiMat.c3, aiMat.d3,
            aiMat.a4, aiMat.b4, aiMat.c4, aiMat.d4
    };
}

static inline glm::vec3 GetGLMVec(const aiVector3D& vec)
{
    return {vec.x, vec.y, vec.z};
}

static inline glm::quat GetGLMQuat(const aiQuaternion& pOrientation)
{
    return {pOrientation.w, pOrientation.x, pOrientation.y, pOrientation.z};
}

namespace DemonAnimation {

    struct BoneInfo
    {
        int id;
        glm::mat4 offset;
        glm::mat4 finalTransformation;
    };


    class DA_riggedMesh : public DemonBase::b_Mesh{
    public:
        DA_riggedMesh(Vertex *vertices, unsigned int vertexLen,
                                     unsigned int *indices, unsigned int indexLen,
                                     std::map<std::string, BoneInfo*> *boneMap, std::map<unsigned int, aiAnimation*> *animationMap,
                                     glm::mat4 inverseGlobal, aiNode *rootNode, int body) :
                DemonBase::b_Mesh(vertices, vertexLen, indices, indexLen),
                currentBody(body),
                _rootNode(rootNode),
                _rootTransformation(inverseGlobal),
                _boneMap(*boneMap),
                _animationMap(*animationMap),
                _PTRanimationMap(animationMap),
                _PTRboneMap(boneMap){
            currentAnim = animationMap->at(0);
        }

        virtual ~DA_riggedMesh();

        void setAnimation(unsigned int index);

        // WARNING, VERY SLOW, DON'T USE IN REAL TIME
        void setAnimation(std::string name);

        void setShader(DGL::Shader *newShader);

        static double GetScaleFactor(double last, double next, double currentTime);

        static glm::mat4 guessPosition(float time, glm::vec3 pos1, double pos1time,
                                glm::vec3 pos2, double pos2time);

        static glm::mat4 guessRotation(float time, glm::quat pos1, double pos1time,
                                glm::quat pos2, double pos2time);

        static unsigned int GetPositionIndex(float animationTime, unsigned int numPositions, aiVectorKey *keys);

        static unsigned int GetPositionIndex(float animationTime, unsigned int numPositions, aiQuatKey *keys);

        void assignTransformations(float currentTime, aiNode *currentNode, glm::mat4 currentParentTransform);

        void doAnimation(float time);

        void playOnce(float currentTime);

        int animationFinished(float time);


    private:

        float endTime = 1.0f;
        float _Time = 0.0f;
        float referenceTime = 0.0f;
        int currentBody = 0;


        DGL::Shader *currentShader = nullptr;

        aiNode *_rootNode = nullptr;
        aiAnimation *currentAnim = nullptr;

        glm::mat4 _rootTransformation = glm::mat4(0.0f);

        // Bone/Node Name:BoneID
        std::map<std::string, BoneInfo*> _boneMap;
        // Name:Animation
        std::map<unsigned int, aiAnimation*> _animationMap;

        // These hold the pointers we can destroy later
        std::map<unsigned int, aiAnimation*> *_PTRanimationMap;
        std::map<std::string, BoneInfo*> *_PTRboneMap;


    };

} // DemonAnimation

#endif //DEMONENGINE_DA_RIGGEDMESH_H
