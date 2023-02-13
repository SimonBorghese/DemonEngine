//
// Created by simon on 2/3/23.
//

#ifndef DEMONENGINE_LEVEL_H
#define DEMONENGINE_LEVEL_H
#include <PathFinder/Dijkstra.h>
#include <PathFinder/PathFinder.h>
#include "NPC_Node.h"
#include "Path.h"
#include <vector>
#include <DemonPhysics/DP_PhysicsManager.h>
#include <SDL2/SDL.h>

namespace DNPC {

    class Level {
    public:
        Level(DemonPhysics::DP_PhysicsManager *dpPhysicsManager);
        virtual ~Level();

        void addNode(glm::vec3 position);
        // Note very intensive
        void removeNode(glm::vec3 position);
        void removeNode(NPC_Node *targetNode);
        NPC_Node* getNodeNear(glm::vec3 position);

        Path* getPath(NPC_Node *start, NPC_Node *end);
        Path* getPath(glm::vec3 start, glm::vec3 end);

    private:
        std::vector<NPC_Node*> _nodes;
        DemonPhysics::DP_PhysicsManager *_physicsManager;
        std::map<std::pair<NPC_Node*, NPC_Node*>, std::vector<NPC_Node*>*> _cachePaths;
    };

} // DNPC

#endif //DEMONENGINE_LEVEL_H
