//
// Created by simon on 5/28/22.
//

#ifndef DEMONENGINE_DR_SHADER_H
#define DEMONENGINE_DR_SHADER_H

#include <glad/glad.h>
#include <stdio.h>
#include <glm/glm.hpp>

namespace DemonRender {

    class DR_Shader {
    public:
        DR_Shader() {}

        void createProgram(const char *vertexFile, const char *fragmentFile);

        void useProgram();

        void destroyProgram();

        uint32_t getUniformLocation(const char *uniform);

        void bindMatrix4f(uint32_t location, glm::mat4 targetMat);

        void bindVector3(uint32_t location, glm::vec3 targetVec);

        void bindFloat(uint32_t loc, float value) { glUniform1f(loc, value); }

        void bindInt(GLuint loc, int value) { glUniform1i(loc, value); }

        void bindDiffuseTexture(GLuint targetTexture);


    private:
        GLuint shaderProgram = 0;

        GLuint diffuseLocation = 0;

    };

} // DemonRender

#endif //DEMONENGINE_DR_SHADER_H
