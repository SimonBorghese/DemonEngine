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

#include <DemonGame/Master/Engine.h>

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
                _rootTransformation(inverseGlobal),
                _boneMap(*boneMap),
                _animationMap(*animationMap),
                _PTRboneMap(boneMap),
                _PTRanimationMap(animationMap),
                _rootNode(rootNode),
                currentBody(body){
            currentAnim = animationMap->at(0);
        }

        ~DA_riggedMesh(){
            for (unsigned int b = 0; b < _PTRboneMap->size(); b++){
                //(_RboneMap->begin() + _RboneMap.)
            }
            delete _PTRboneMap;
            delete _PTRanimationMap;
        }

        void setAnimation(unsigned int index){
            currentAnim = _animationMap.at(index);
        }

        void setShader(DemonRender::DR_Shader *newShader){
            currentShader = newShader;
        }

        static double GetScaleFactor(double last, double next, double currentTime)
        {
            double middle = currentTime - last;
            double timeDelay = next - last;
            return middle / timeDelay;
        }

        static glm::mat4 guessPosition(float time, glm::vec3 pos1, double pos1time,
                                glm::vec3 pos2, double pos2time){
                double scaleFactor = GetScaleFactor(pos1time,
                                                   pos2time, time);
                glm::vec3 finalPosition = glm::mix(pos1,
                                                   pos2, scaleFactor);
                return glm::translate(glm::mat4(1.0f), finalPosition);
        }

        static glm::mat4 guessRotation(float time, glm::quat pos1, double pos1time,
                                glm::quat pos2, double pos2time){
            double scaleFactor = GetScaleFactor(pos1time,
                                               pos2time, time);
            glm::quat finalPosition = glm::slerp(pos1,
                                               pos2, (float) scaleFactor);
            return (glm::mat4(glm::normalize(finalPosition)));
        }

        static unsigned int GetPositionIndex(float animationTime, unsigned int numPositions, aiVectorKey *keys)
        {
            for (int index = -1; index < (int) numPositions; index++)
            {
                if (animationTime < keys[index + 1].mTime)
                    return index;
            }
            return numPositions - 2;
        }
        static unsigned int GetPositionIndex(float animationTime, unsigned int numPositions, aiQuatKey *keys)
        {
            for (int index = -1; index < (int) numPositions; index++)
            {
                if (animationTime < keys[index + 1].mTime)
                    return index;
            }
            return numPositions - 2;
        }

        void assignTransformations(float currentTime, aiNode *currentNode, glm::mat4 currentParentTransform){
            aiNodeAnim *currentCurve = nullptr;

            glm::mat4 currentNodeTransform = GetGLMMat(currentNode->mTransformation);
            //glm::mat4 currentNodeTransform = glm::mat4(1.0f);
            for (unsigned int c = 0; c < currentAnim->mNumChannels; c++){
                if (currentAnim->mChannels[c]->mNodeName == currentNode->mName){
                    currentCurve = currentAnim->mChannels[c];
                    break;
                }
            }

            if (currentCurve){
                glm::mat4 positionMat = glm::mat4(1.0f);
                glm::mat4 rotationMat = glm::mat4(1.0f);
                glm::mat4 scalingMat = glm::mat4(1.0f);

                if (currentCurve->mNumScalingKeys > 1) {
                    unsigned int index = GetPositionIndex(currentTime, currentCurve->mNumScalingKeys,
                                                 currentCurve->mScalingKeys);
                    if (index < currentCurve->mNumScalingKeys - 1) {
                        scalingMat = guessPosition(currentTime, GetGLMVec(currentCurve->mScalingKeys[index].mValue),
                                                   currentCurve->mScalingKeys[index].mTime,
                                                   GetGLMVec(currentCurve->mScalingKeys[index + 1].mValue),
                                                   currentCurve->mScalingKeys[index + 1].mTime);
                    }
                } else{
                    scalingMat = glm::scale(glm::mat4(1.0f), GetGLMVec(currentCurve->mScalingKeys[0].mValue) );
                }

                if (currentCurve->mNumPositionKeys > 1) {
                    unsigned int index = GetPositionIndex(currentTime, currentCurve->mNumPositionKeys,
                                                 currentCurve->mPositionKeys);
                    if (index < currentCurve->mNumPositionKeys - 1) {
                        positionMat = guessPosition(currentTime, GetGLMVec(currentCurve->mPositionKeys[index].mValue),
                                                    currentCurve->mPositionKeys[index].mTime,
                                                    GetGLMVec(currentCurve->mPositionKeys[index + 1].mValue),
                                                    currentCurve->mPositionKeys[index + 1].mTime);
                    }
                }
                else{
                    positionMat = glm::translate(glm::mat4(1.0f), GetGLMVec(currentCurve->mPositionKeys[0].mValue));
                }

                if (currentCurve->mNumRotationKeys > 1) {
                    unsigned int index = GetPositionIndex(currentTime, currentCurve->mNumRotationKeys,
                                                 currentCurve->mRotationKeys);
                    if (index < currentCurve->mNumRotationKeys - 1){
                        rotationMat = guessRotation(currentTime, GetGLMQuat(currentCurve->mRotationKeys[index].mValue),
                                                currentCurve->mRotationKeys[index].mTime,
                                                GetGLMQuat(currentCurve->mRotationKeys[index + 1].mValue),
                                                currentCurve->mRotationKeys[index + 1].mTime);
                    }
                } else{
                    rotationMat = glm::mat4(GetGLMQuat(currentCurve->mRotationKeys[0].mValue) );
                }


                glm::mat4 newTransform = glm::mat4(1.0f);
                newTransform = positionMat * rotationMat * scalingMat;
                currentNodeTransform = newTransform;

            }
            glm::mat4 globalTransform = currentParentTransform * currentNodeTransform;

            auto found = _boneMap.find(std::string(currentNode->mName.C_Str()));
            if (found != _boneMap.end()){
                BoneInfo *currentBoneInfo = found->second;
                currentBoneInfo->finalTransformation =  globalTransform * currentBoneInfo->offset;

                // TO-DO: SEPERATE UNIFORM GET LOCATION TO ANOTHER PLACE FOR BETTER PERFORMANCE
                assert(currentShader != nullptr);
                //printf("Current body: %d\n", currentBody);

                currentShader->bindInt(currentShader->getUniformLocation("currentBody"), currentBody);

                currentShader->bindMatrix4f(
                        currentShader->getUniformLocation(
                                fmt::format("bodyTransforms[{}][{}]", currentBody, currentBoneInfo->id).c_str()), (currentBoneInfo->finalTransformation));
            }
            else{
                //printf("Node: %s did not have a bone\n", currentNode->mName.C_Str());
            }

            for (unsigned int ch = 0; ch < currentNode->mNumChildren; ch++){
                assignTransformations(currentTime, currentNode->mChildren[ch], globalTransform);
            }
        }

        void doAnimation(float time){
            //assignTransformations(fmod(time, currentAnim->mDuration), _rootNode, glm::mat4(1.0f));
            if (endTime < 0.0f || (time * currentAnim->mTicksPerSecond) - (referenceTime) <= endTime) {
                printf("Current time: %f end time: %f\n", (time * currentAnim->mTicksPerSecond) - (referenceTime), endTime);
                // 5.000 24.000
                //printf("Duration: %f, ticks per second: %f\n", currentAnim->mDuration, currentAnim->mTicksPerSecond);
                assignTransformations(std::clamp( (float) ((time * currentAnim->mTicksPerSecond) - (referenceTime)), 0.0f, (float) currentAnim->mDuration), _rootNode,
                                      glm::mat4(1.0f));
            }
            else if ( (time * currentAnim->mTicksPerSecond) - (referenceTime) >= endTime){
                assignTransformations(fmod(currentAnim->mDuration * currentAnim->mTicksPerSecond, currentAnim->mDuration), _rootNode,
                                      glm::mat4(1.0f));
            }
        }

        void playOnce(float currentTime){
            referenceTime = currentTime * currentAnim->mTicksPerSecond;
            endTime = (currentAnim->mDuration);
        }

        int animationFinished(float time){
            return (((time * currentAnim->mTicksPerSecond) - (referenceTime)) >= currentAnim->mDuration) || referenceTime == 0.0f;
        }


        aiAnimation *currentAnim = nullptr;

    private:

        float endTime = 1.0f;

        float referenceTime = 0.0f;
        int currentBody = 0;

        float _Time = 0.0f;
        DemonRender::DR_Shader *currentShader = nullptr;

        aiNode *_rootNode = nullptr;

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
