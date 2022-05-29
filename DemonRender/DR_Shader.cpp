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
        GLint id;
        glGetIntegerv(GL_CURRENT_PROGRAM,&id);
        //if ((GLuint) id != shaderProgram) {
            glUseProgram(shaderProgram);
        //}

    }

    void DR_Shader::destroyProgram(){
        glDeleteProgram(shaderProgram);
    }
} // DemonRender