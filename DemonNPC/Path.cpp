//
// Created by simon on 2/3/23.
//

#include "Path.h"

namespace DNPC {
    Path::Path(std::vector<NPC_Node*> *path, glm::vec3 destination) : _path(path), _position(0.0f), _destination(destination){
        //assert(_path->size() > 0);
    }

    void Path::setStartPosition(glm::vec3 position){
        _position = position;
    }
    glm::vec3 Path::getNextTarget(){
        if (!_path->empty()){
            return _path->at(_path->size() - 1)->getPosition();
        }

        return _destination;
    }
    void Path::advanceTarget(){
        if (!_path->empty()) {
            _path->pop_back();
        }
        //assert(_path->size() > 0);
        //return  _path->at(_path->size() - 1)->getPosition();
    }

} // DNPC