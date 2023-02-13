//
// Created by simon on 2/3/23.
//

#ifndef DEMONENGINE_PATH_H
#define DEMONENGINE_PATH_H
#include "NPC_Node.h"
#include <vector>

namespace DNPC {

    class Path {
    public:
        Path(std::vector<NPC_Node*> *path, glm::vec3 destination);

        void setStartPosition(glm::vec3 position);
        glm::vec3 getNextTarget();
        void advanceTarget();


    private:
        std::vector<NPC_Node*> *_path;
        glm::vec3 _position;
        glm::vec3 _destination;
    };

} // DNPC

#endif //DEMONENGINE_PATH_H
