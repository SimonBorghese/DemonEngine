//
// Created by simon on 8/16/22.
//

#include "DA_riggedMesh.h"

namespace DemonAnimation {

    DA_riggedMesh::~DA_riggedMesh(){
    for (unsigned int b = 0; b < _PTRboneMap->size(); b++){
        //(_RboneMap->begin() + _RboneMap.)
    }
    delete _PTRboneMap;
    delete _PTRanimationMap;
}

void DA_riggedMesh::setAnimation(unsigned int index){
        currentAnim = _animationMap.at(index);
}

    std::vector<std::string> split(const std::string& s, char seperator)
    {
        std::vector<std::string> output;
        std::string::size_type prev_pos = 0, pos = 0;
        while((pos = s.find(seperator, pos)) != std::string::npos)
        {
            std::string substring( s.substr(prev_pos, pos-prev_pos) );
            output.push_back(substring);
            prev_pos = ++pos;
        }
        output.push_back(s.substr(prev_pos, pos-prev_pos)); // Last word
        return output;
    }

void DA_riggedMesh::setAnimation(std::string name){
        auto thisCurrentAnim = _animationMap.begin();
        while (thisCurrentAnim != _animationMap.end()){
            std::string nameStr = split(std::string(thisCurrentAnim->second->mName.C_Str()), '|').at(1);

            if (!strcmp(name.c_str(), nameStr.c_str())){
                currentAnim = thisCurrentAnim->second;
                return;
            }
            ++thisCurrentAnim;
        }
        assert(0);
}

void DA_riggedMesh::setShader(DGL::Shader *newShader){
    currentShader = newShader;
}



double DA_riggedMesh::GetScaleFactor(double last, double next, double currentTime)
{
    double middle = currentTime - last;
    double timeDelay = next - last;
    return middle / timeDelay;
}

glm::mat4 DA_riggedMesh::guessPosition(float time, glm::vec3 pos1, double pos1time,
                               glm::vec3 pos2, double pos2time){
    double scaleFactor = GetScaleFactor(pos1time,
                                        pos2time, time);
    glm::vec3 finalPosition = glm::mix(pos1,
                                       pos2, scaleFactor);
    return glm::translate(glm::mat4(1.0f), finalPosition);
}

glm::mat4 DA_riggedMesh::guessRotation(float time, glm::quat pos1, double pos1time,
                               glm::quat pos2, double pos2time){
    double scaleFactor = GetScaleFactor(pos1time,
                                        pos2time, time);
    glm::quat finalPosition = glm::slerp(pos1,
                                         pos2, (float) scaleFactor);
    return (glm::mat4(glm::normalize(finalPosition)));
}

unsigned int DA_riggedMesh::GetPositionIndex(float animationTime, unsigned int numPositions, aiVectorKey *keys)
{
    for (int index = -1; index < (int) numPositions; index++)
    {
        if (animationTime < keys[index + 1].mTime)
            return index;
    }
    return numPositions - 2;
}

unsigned int DA_riggedMesh::GetPositionIndex(float animationTime, unsigned int numPositions, aiQuatKey *keys)
{
    for (int index = -1; index < (int) numPositions; index++)
    {
        if (animationTime < keys[index + 1].mTime)
            return index;
    }
    return numPositions - 2;
}

void DA_riggedMesh::assignTransformations(float currentTime, aiNode *currentNode, glm::mat4 currentParentTransform){
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
        currentShader->useShader();

        currentShader->uniformInt("currentBody", currentBody);

        currentShader->uniformMat4(
                        fmt::format("bodyTransforms[{}][{}]", currentBody, currentBoneInfo->id).c_str(), (currentBoneInfo->finalTransformation));
    }
    else{
        //printf("Node: %s did not have a bone\n", currentNode->mName.C_Str());
    }

    for (unsigned int ch = 0; ch < currentNode->mNumChildren; ch++){
        assignTransformations(currentTime, currentNode->mChildren[ch], globalTransform);
    }
}

void DA_riggedMesh::doAnimation(float time){
    //assignTransformations(fmod(time, currentAnim->mDuration), _rootNode, glm::mat4(1.0f));
    if (endTime < 0.0f || (time * currentAnim->mTicksPerSecond) - (referenceTime) <= endTime) {
        //printf("Current time: %f end time: %f\n", (time * currentAnim->mTicksPerSecond) - (referenceTime), endTime);
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

void DA_riggedMesh::playOnce(float currentTime){
    referenceTime = currentTime * currentAnim->mTicksPerSecond;
    endTime = (currentAnim->mDuration);
}

int DA_riggedMesh::animationFinished(float time){
    return (((time * currentAnim->mTicksPerSecond) - (referenceTime)) >= currentAnim->mDuration) || referenceTime == 0.0f;
}
} // DemonAnimation