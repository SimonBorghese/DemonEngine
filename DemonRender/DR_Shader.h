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

    private:
        GLuint shaderProgram;

    };

} // DemonRender

#endif //DEMONENGINE_DR_SHADER_H
