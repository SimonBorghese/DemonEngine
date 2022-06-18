//
// Created by simon on 5/28/22.
//

#include "DR_Shader.h"

namespace DemonRender {

    void DR_Shader::createProgram(const char *vertexFile, const char *fragmentFile){
        FILE *file = fopen(vertexFile, "r");
        int fLength = 0;
        fseek(file, 0, SEEK_END);
        fLength = ftell(file);
        fseek(file, 0, SEEK_SET);
        char *vertexShaderSource = (char*) malloc(sizeof(char) * fLength + 1);
        fread(vertexShaderSource, 1, fLength, file);
        vertexShaderSource[fLength] = '\0';
        fclose(file);

        file = fopen(fragmentFile, "r");
        fseek(file, 0, SEEK_END);
        fLength = ftell(file);
        fseek(file, 0, SEEK_SET);
        char *fragmentShaderSource = (char*) malloc(sizeof(char) * fLength + 1);
        fread(fragmentShaderSource, 1, fLength, file);
        fragmentShaderSource[fLength] = '\0';
        fclose(file);

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        shaderProgram = glCreateProgram();

        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);


        glCompileShader(vertexShader);
        glCompileShader(fragmentShader);

        int success;
        char infoLog[512];

        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success){
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            printf("Vertex Shader Error: %s\n", infoLog);
        }

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success){
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            printf("Fragment Shader Error: %s\n", infoLog);
        }

        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);

        glLinkProgram(shaderProgram);

        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success){
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            printf("Main Error: %s\n", infoLog);
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        free(vertexShaderSource);
        free(fragmentShaderSource);


    }
    void DR_Shader::useProgram(){
        glUseProgram(shaderProgram);
        if (diffuseLocation == 0){
            diffuseLocation = getUniformLocation("diffuse_texture");
        }
    }

    void DR_Shader::destroyProgram(){
        glDeleteProgram(shaderProgram);
    }

    uint32_t DR_Shader::getUniformLocation(const char *uniform){
        return glGetUniformLocation(shaderProgram, uniform);
    }
    void DR_Shader::bindMatrix4f(uint32_t location, glm::mat4 targetMat){
        glUniformMatrix4fv(location, 1, GL_FALSE, &targetMat[0][0]);
    }

    void DR_Shader::bindVector3(uint32_t location, glm::vec3 targetVec){
        glUniform3fv(location, 1, &targetVec[0]);
    }

    void DR_Shader::bindDiffuseTexture(GLuint targetTexture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, targetTexture);
        bindInt(diffuseLocation, 0);
    }
} // DemonRender