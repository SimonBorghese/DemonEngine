//
// Created by simon on 1/10/23.
//

#include "Shader.h"

namespace DGL {
    GLuint Shader::_transformBlock = 0;
    struct transformBlock Shader::_globalTransform = {glm::mat4(1.0f), glm::mat4(1.0f)};

    GLuint Shader::_dynamicLightUBO = 0;
    struct dynamicLights Shader::_dynamicLights;

    GLuint Shader::_texturesUBO = 0;
    struct _textureStruct Shader::_textures;


    Shader::Shader(const char *vertex, const char *fragment){
        _shaders[0] = vertex;
        _shaders[1] = fragment;
        _shaders[2] = "";
    }
    Shader::Shader(const char *vertex, const char *geometry, const char *fragment){
        _shaders[0] = vertex;
        _shaders[1] = fragment;
        _shaders[2] = geometry;
    }

    Shader::~Shader() = default;

    void Shader::createShader(){
        FILE *file = fopen(_shaders[0].c_str(), "r");
        int fLength;
        fseek(file, 0, SEEK_END);
        fLength = (int) ftell(file);
        fseek(file, 0, SEEK_SET);
        char *vertexShaderSource = (char *) malloc(sizeof(char) * fLength + 1);
        fread(vertexShaderSource, 1, fLength, file);
        vertexShaderSource[fLength] = '\0';
        fclose(file);

        file = fopen(_shaders[1].c_str(), "r");
        fseek(file, 0, SEEK_END);
        fLength = (int) ftell(file);
        fseek(file, 0, SEEK_SET);
        char *fragmentShaderSource = (char *) malloc(sizeof(char) * fLength + 1);
        fread(fragmentShaderSource, 1, fLength, file);
        fragmentShaderSource[fLength] = '\0';
        fclose(file);

        char *geometryShaderSource = nullptr;

        if (strcmp(_shaders[2].c_str(), "") != 0){
            file = fopen(_shaders[2].c_str(), "r");
            fseek(file, 0, SEEK_END);
            fLength = (int) ftell(file);
            fseek(file, 0, SEEK_SET);
            geometryShaderSource = (char *) malloc(sizeof(char) * fLength + 1);
            fread(geometryShaderSource, 1, fLength, file);
            geometryShaderSource[fLength] = '\0';
            fclose(file);
        }

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        GLuint geometryShader = 0;
        if (geometryShaderSource){
            geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        }
        _shaderProgram = glCreateProgram();

        glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
        if (geometryShader){
            glShaderSource(geometryShader, 1, &geometryShaderSource, nullptr);
        }


        glCompileShader(vertexShader);
        glCompileShader(fragmentShader);
        if (geometryShader){
            glCompileShader(geometryShader);
        }

        int success;
        char infoLog[512];

        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
            ALERT("FAILED TO BUILD VERTEX SHADER!\n");
            ALERT(infoLog);
            assert(0);
        }

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
            ALERT("FAILED TO BUILD FRAGMENT SHADER!\n");
            ALERT(infoLog);
            assert(0);
        }

        if (geometryShader) {
            glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(geometryShader, 512, nullptr, infoLog);
                ALERT("FAILED TO BUILD GEOMETRY SHADER!\n");
                ALERT(infoLog);
                assert(0);
            }
        }

        glAttachShader(_shaderProgram, vertexShader);
        glAttachShader(_shaderProgram, fragmentShader);
        if (geometryShader){
            glAttachShader(_shaderProgram, geometryShader);
        }

        glLinkProgram(_shaderProgram);
        glUseProgram(_shaderProgram);

        glGetProgramiv(_shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(_shaderProgram, 512, nullptr, infoLog);
            ALERT("FAILED TO LINK SHADER!\n");
            ALERT(infoLog);
            assert(0);
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        if (geometryShader){
            glDeleteShader(geometryShader);
        }

        free(vertexShaderSource);
        free(fragmentShaderSource);
        if (geometryShaderSource){
            free(geometryShaderSource);
        }

        // Construct UBO if not already done so
        if (!_transformBlock){
            glGenBuffers(1, &_transformBlock);
            glBindBuffer(GL_UNIFORM_BUFFER, _transformBlock);
            glBufferData(GL_UNIFORM_BUFFER, sizeof(struct transformBlock), &_globalTransform, GL_STREAM_DRAW);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
        }

        if (!_dynamicLightUBO){
            glGenBuffers(1, &_dynamicLightUBO);
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, _dynamicLightUBO);
            glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(struct dynamicLights), &_dynamicLights, GL_DYNAMIC_COPY);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, _dynamicLightUBO);
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
        }

        if (!_texturesUBO){
            glGenBuffers(1, &_texturesUBO);
            glBindBuffer(GL_UNIFORM_BUFFER, _texturesUBO);
            glBufferData(GL_UNIFORM_BUFFER, sizeof(struct _textureStruct), &_textures, GL_STREAM_DRAW);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
        }


    }
    void Shader::useShader() const{
        glUseProgram(_shaderProgram);

    }
    void Shader::destroyShader() const{
        glUseProgram(0);
        glDeleteProgram(_shaderProgram);
    }

    void Shader::bindGlobals(){
        useShader();
        if (!_transformLocation) {
            _transformLocation = glGetUniformBlockIndex(_shaderProgram, "Transform");
        }
        if (!_dynamicLightsLocation) {
            _dynamicLightsLocation = glGetProgramResourceIndex(_shaderProgram, GL_SHADER_STORAGE_BLOCK, "dynamicLights");
        }
        if (!_texturesLocation){
            _texturesLocation = glGetUniformBlockIndex(_shaderProgram, "Textures");
        }
        if (_transformLocation != (GLuint) GL_INVALID_INDEX) {
            glUniformBlockBinding(_shaderProgram, _transformLocation, 0);
            glBindBufferBase(GL_UNIFORM_BUFFER, 0, _transformBlock);
        }
        if (_dynamicLightsLocation != (GLuint) GL_INVALID_INDEX) {
            glShaderStorageBlockBinding(_shaderProgram, _dynamicLightsLocation, 1);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, _dynamicLightUBO);
        }
        if (_texturesLocation != (GLuint) GL_INVALID_INDEX){
            glUniformBlockBinding(_shaderProgram, _texturesLocation, 2);
            glBindBufferBase(GL_UNIFORM_BUFFER, 2, _texturesUBO);
        }
    }

    void Shader::setView(glm::mat4 view){
        if (_globalTransform.view == view) { return; }
        _globalTransform.view = view;
        remakeGlobals();
    }
    void Shader::setProjection(glm::mat4 projection){
        if (_globalTransform.projection == projection) { return;}
        _globalTransform.projection = projection;
        remakeGlobals();
    }
    void Shader::remakeGlobals(){
        glBindBuffer(GL_UNIFORM_BUFFER, _transformBlock);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(struct transformBlock), &_globalTransform);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glBindBuffer(GL_UNIFORM_BUFFER, _dynamicLightUBO);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(struct dynamicLights), &_dynamicLights);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void Shader::remakeTextures(){
        glBindBuffer(GL_UNIFORM_BUFFER, _texturesUBO);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(struct _textureStruct), &_textures);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

    }

    void Shader::uniformMat4(std::string location, glm::mat4 target){
        useShader();
        glUniformMatrix4fv((int) getUniformLocation(location), 1, GL_FALSE, glm::value_ptr(target));
    }
    void Shader::uniformInt(std::string location, glm::int32 target){
        useShader();
        glUniform1i((int) getUniformLocation(location), target);
    }

    void Shader::uniformVec3(std::string location, glm::vec3 target){
        useShader();
        glUniform3fv((int) getUniformLocation(location), 1, glm::value_ptr(target));
    }
    void Shader::uniformFloat(std::string location, glm::float32 target){
        useShader();
        glUniform1f((int) getUniformLocation(location), target);
    }

    GLuint Shader::getUniformLocation(std::string uniform){
        useShader();
        auto locationFound = _locationCache.find(uniform);
        if (locationFound != _locationCache.end()) { return locationFound->second; }
        GLuint location = glGetUniformLocation(_shaderProgram, uniform.c_str());
        _locationCache.insert(std::pair<std::string, GLuint>(uniform, location));
        return location;
    }
} // DGL