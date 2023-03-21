//
// Created by simon on 3/9/23.
//

#include "FileSystem.h"

namespace DFS {
    FileSystem *FileSystem::_fileSysConfig = new FileSystem(GAME_NAME);

    FileSystem::FileSystem(std::string gamePrefix) : _gamePrefix(gamePrefix) {}

    void FileSystem::setGamePrefix(std::string prefix) { _gamePrefix = std::string(prefix); }

    std::string FileSystem::loadShader(std::string fileName) {
        std::string _shaderPath = fmt::format("{}/{}/{}", _gamePrefix, GAME_SHADER_DIRECTORY, fileName);
        FILE *file = fopen(_shaderPath.c_str(), "r");
        int fLength;
        fseek(file, 0, SEEK_END);
        fLength = (int) ftell(file);
        fseek(file, 0, SEEK_SET);
        char *vertexShaderSource = (char *) malloc(sizeof(char) * fLength + 1);
        fread(vertexShaderSource, 1, fLength, file);
        vertexShaderSource[fLength] = '\0';
        fclose(file);

        // TODO: FIX THIS MEMORY LEAK!!!!!!!!!!!!!!!!!!!!!!!
        std::string *_shaderSource = new std::string(vertexShaderSource, fLength + 1);
        free(vertexShaderSource);
        return *_shaderSource;
    }

    std::string FileSystem::getMapGeometryPath(std::string mapName) {
        return fmt::format("{}/Maps/{}/{}.obj", _gamePrefix, mapName, mapName);
    }

    std::string FileSystem::getMapBSPPath(std::string mapName) {
        return fmt::format("{}/Maps/{}/{}.bsp", _gamePrefix, mapName, mapName);
    }

    std::string FileSystem::getModelPath(std::string modelName) {
        return fmt::format("{}/Models/{}/{}.fbx", _gamePrefix, modelName, modelName);
    }

    std::string FileSystem::getTexturePath(std::string modelName, std::string textureName) {
        std::string targetFile = fmt::format("{}/Models/{}/{}", _gamePrefix, modelName, textureName);

        if (!access(targetFile.c_str(), F_OK)) {
            return targetFile;
        } else {
            return fmt::format("{}/Models/{}/{}", _gamePrefix, "level0", textureName);
        }
    }

    FileSystem *FileSystem::getFS() {
        return _fileSysConfig;
    }
} // DFS