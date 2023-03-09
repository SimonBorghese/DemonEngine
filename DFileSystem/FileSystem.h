//
// Created by simon on 3/9/23.
//

#ifndef DEMONENGINE_FILESYSTEM_H
#define DEMONENGINE_FILESYSTEM_H

#include <DemonGame/GAME_DEF.h>
#include <stdio.h>
#include <fmt/format.h>
#include <string>
/*
 * DFS: Provides a wrapper for loading in data from game directories.
 * Directory hard coded in game binary
 */

namespace DFS {

    class FileSystem {
    public:
        FileSystem(std::string gamePrefix);

        void setGamePrefix(std::string prefix);

        std::string loadShader(std::string fileName);

        std::string getMapGeometryPath(std::string mapName);

        std::string getMapBSPPath(std::string mapName);

        std::string getModelPath(std::string modelName);

        static FileSystem *getFS();

    private:
        static FileSystem *_fileSysConfig;
        std::string _gamePrefix;


    };

} // DFS

#endif //DEMONENGINE_FILESYSTEM_H
