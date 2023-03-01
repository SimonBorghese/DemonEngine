//
// Created by simon on 1/12/23.
//

#include "Light.h"

namespace DGL {
    Light::Light(){
        _lightNumber = DGL::Shader::_dynamicLights.numLights; DGL::Shader::_dynamicLights.numLights++;

        _lightConfig.position =         &DGL::Shader::_dynamicLights.position[_lightNumber];
        _lightConfig.direction =        &DGL::Shader::_dynamicLights.direction[_lightNumber];
        _lightConfig.cutOff =           &DGL::Shader::_dynamicLights.cutOff[_lightNumber];
        _lightConfig.outerCutOff =      &DGL::Shader::_dynamicLights.outerCutOff[_lightNumber];
        _lightConfig.distance =         &DGL::Shader::_dynamicLights.distance[_lightNumber];
        _lightConfig.colour =           &DGL::Shader::_dynamicLights.colour[_lightNumber];
        _lightConfig.ambient =          &DGL::Shader::_dynamicLights.ambient[_lightNumber];
        _lightConfig.specularStrength = &DGL::Shader::_dynamicLights.specularStrength[_lightNumber];
        _lightConfig.specValue =        &DGL::Shader::_dynamicLights.specValue[_lightNumber];
        _lightConfig.enableShadow =     &DGL::Shader::_dynamicLights.enableShadow[_lightNumber];

        *_lightConfig.position = glm::vec3(0.0f);
        *_lightConfig.direction = glm::vec3(0.0f);
        *_lightConfig.cutOff = 0.0f;
        *_lightConfig.outerCutOff = 0.0f;
        *_lightConfig.distance = 1.0f;
        *_lightConfig.colour = glm::vec3(1.0f);
        *_lightConfig.ambient = 0.1f;
        *_lightConfig.specularStrength = 0.5f;
        *_lightConfig.specValue = 32;
        *_lightConfig.enableShadow = 0;
    }

    void Light::createShadowBuffer(uint32_t width, uint32_t height){
        _shadowWidth = width;
        _shadowHeight = height;
        glGenFramebuffers(1, &_shadowBuffer);
        glGenTextures(1, &_shadowTexture);


        glBindTexture(GL_TEXTURE_CUBE_MAP, _shadowTexture);
        for (unsigned int i = 0; i < 6; ++i)
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
                         (int) _shadowWidth, (int) _shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BORDER_COLOR, borderColor);
        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

        glBindFramebuffer(GL_FRAMEBUFFER, _shadowBuffer);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _shadowTexture, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glm::mat4 lightProjection = glm::perspective(glm::radians(90.0f), (float)_shadowWidth/(float)_shadowHeight, 1.0f, getDistance());

        shadowTransforms.clear();
        shadowTransforms.push_back(lightProjection *
                                   glm::lookAt(getPosition(), getPosition() + glm::vec3( 1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)));
        shadowTransforms.push_back(lightProjection *
                                   glm::lookAt(getPosition(), getPosition() + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)));
        shadowTransforms.push_back(lightProjection *
                                   glm::lookAt(getPosition(), getPosition() + glm::vec3( 0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
        shadowTransforms.push_back(lightProjection *
                                   glm::lookAt(getPosition(), getPosition() + glm::vec3( 0.0,-1.0, 0.0), glm::vec3(0.0, 0.0,-1.0)));
        shadowTransforms.push_back(lightProjection *
                                   glm::lookAt(getPosition(), getPosition() + glm::vec3( 0.0, 0.0, 1.0), glm::vec3(0.0,-1.0, 0.0)));
        shadowTransforms.push_back(lightProjection *
                                   glm::lookAt(getPosition(), getPosition() + glm::vec3( 0.0, 0.0,-1.0), glm::vec3(0.0,-1.0, 0.0)));
        *_lightConfig.enableShadow = 1;
    }

    void Light::renderShadowBuffer(DGL::Shader *shadowShader, std::function<void()> renderFunction){
        if (*_lightConfig.enableShadow && !enableShadows){
            glActiveTexture(GL_TEXTURE2 + _lightNumber);
            glBindTexture(GL_TEXTURE_CUBE_MAP, _shadowTexture);
            return;
        }
        if (!*_lightConfig.enableShadow) { return; }
        // 1. first render to depth map
        glViewport(0, 0, (int) _shadowWidth, (int) _shadowHeight);
        glBindFramebuffer(GL_FRAMEBUFFER, _shadowBuffer);
        glClear(GL_DEPTH_BUFFER_BIT);

        glm::mat4 lightProjection = glm::perspective(glm::radians(90.0f), (float)_shadowWidth/(float)_shadowHeight, 1.0f, getDistance());

        shadowTransforms.clear();

        shadowTransforms.push_back(lightProjection *
                                   glm::lookAt(getPosition(), getPosition() + glm::vec3( 1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)));
        shadowTransforms.push_back(lightProjection *
                                   glm::lookAt(getPosition(), getPosition() + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)));
        shadowTransforms.push_back(lightProjection *
                                   glm::lookAt(getPosition(), getPosition() + glm::vec3( 0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
        shadowTransforms.push_back(lightProjection *
                                   glm::lookAt(getPosition(), getPosition() + glm::vec3( 0.0,-1.0, 0.0), glm::vec3(0.0, 0.0,-1.0)));
        shadowTransforms.push_back(lightProjection *
                                   glm::lookAt(getPosition(), getPosition() + glm::vec3( 0.0, 0.0, 1.0), glm::vec3(0.0,-1.0, 0.0)));
        shadowTransforms.push_back(lightProjection *
                                   glm::lookAt(getPosition(), getPosition() + glm::vec3( 0.0, 0.0,-1.0), glm::vec3(0.0,-1.0, 0.0)));

        //depthRender.enableProgram();
        //currentScene.renderAll(&depthRender, lightView, lightProjection);
        shadowShader->useShader();
        shadowShader->uniformVec3("lightPos", getPosition());
        shadowShader->uniformFloat("far_plane", getDistance());
        for (uint s = 0; s < 6; s++){
            shadowShader->uniformMat4(fmt::format("shadowMatrices[{}]", s), shadowTransforms.at(s));
        }

        //glCullFace(GL_FRONT);
        //glPolygonOffset(2.0, 2.0);
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(1.0,2.0);
        renderFunction();
        //glCullFace(GL_BACK);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glActiveTexture(GL_TEXTURE2 + _lightNumber);
        glBindTexture(GL_TEXTURE_CUBE_MAP, _shadowTexture);
        *_lightConfig.enableShadow = 1;
        //enableShadows = 0;
    }

    glm::vec3 Light::getPosition() { return *_lightConfig.position; }
    glm::vec3 Light::getDirection() { return *_lightConfig.direction; }
    float Light::getCutoff() { return *_lightConfig.cutOff; }
    float Light::getOuterCutoff() { return *_lightConfig.outerCutOff; }
    float Light::getDistance() { return *_lightConfig.distance; }
    glm::vec3 Light::getColour() { return *_lightConfig.colour; }
    float Light::getAmbient() { return *_lightConfig.ambient; }
    float Light::getSpecularStrength() { return *_lightConfig.specularStrength; }
    int Light::getSpecularValue() { return *_lightConfig.specValue; }

    void Light::setPosition(glm::vec3 newPos) { *_lightConfig.position = newPos; }
    void Light::setDirection(glm::vec3 newDir) { *_lightConfig.direction = newDir; }
    void Light::setCutoff(float newCut) { *_lightConfig.cutOff = newCut; }
    void Light::setOuterCutoff(float newOutCut) { *_lightConfig.outerCutOff = newOutCut; }
    void Light::setDistance(float newDist) { *_lightConfig.distance = newDist; }
    void Light::setColor(glm::vec3 newColor) { *_lightConfig.colour = newColor; }
    void Light::setAmbient(float newAmbient) { *_lightConfig.ambient = newAmbient; }
    void Light::setSpecularStrength(float newStrength) { *_lightConfig.specularStrength = newStrength; }
    void Light::setSpecularValue(int newSpec) { *_lightConfig.specValue = newSpec; }

    _lightSpec* Light::getConfig(){
        return &_lightConfig;
    }

    void Light::setShadowResolution(uint32_t width, uint32_t height){
        glDeleteFramebuffers(1, &_shadowBuffer);
        glDeleteTextures(1, &_shadowTexture);
        createShadowBuffer(width, height);
    }
} // DGL