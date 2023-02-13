//
// Created by simon on 2/3/23.
//

#ifndef DEMONENGINE_NPC_NODE_H
#define DEMONENGINE_NPC_NODE_H
#include <PathFinder/Dijkstra.h>

namespace DNPC {

    class NPC_Node : public DijkstraNode {
    public:
        NPC_Node(glm::vec3 position = glm::vec3(0.0f)) {
            setPosition(position);
        }

        // Uncomment for AStar support
        //float distanceTo(AStarNode *node) const{
        //    return glm::distance(_position, ((NPC_Node*) node)->getPosition());
        //}

    private:
    };
}
#endif //DEMONENGINE_NPC_NODE_H
