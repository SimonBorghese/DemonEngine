//
// Created by simon on 2/3/23.
//

#include "Level.h"

namespace DNPC {
    Level::Level(DemonPhysics::DP_PhysicsManager *dpPhysicsManager) : _physicsManager(dpPhysicsManager){

    }
    Level::~Level(){

    }

    void Level::addNode(glm::vec3 position){
        // Note 2 self: cache distance
        NPC_Node *newNode = new NPC_Node(position);
        for (uint n = 0; n < _nodes.size(); n++){
            float nodeDistance = glm::distance(position, _nodes.at(n)->getPosition());
            auto rayCast = _physicsManager->sweepCube(position, glm::normalize( _nodes.at(n)->getPosition() - position ), nodeDistance, glm::vec3(1.0f));
            if (!rayCast.hasAnyHits()){
                printf("Node: %ld added child: %d\n", _nodes.size(), n);
                newNode->addChild(_nodes.at(n), nodeDistance);
                _nodes.at(n)->addChild(newNode, nodeDistance);
                assert(newNode);
                assert(_nodes.at(n));
            } else{
                printf("FAILURE OWO!\n");
            }
        }
        _nodes.push_back(newNode);
    }
    // Note very intensive
    void Level::removeNode(glm::vec3 ){

    }
    void Level::removeNode(NPC_Node *){

    }
    NPC_Node* Level::getNodeNear(glm::vec3 position){
        float closestDistance = -1.0f;
        NPC_Node *closestNode = nullptr;
        for (auto cNode : _nodes){
            if (glm::distance(cNode->getPosition(), position) < closestDistance || closestDistance <= -1.0f){
                closestNode = cNode;
                closestDistance = glm::distance(cNode->getPosition(), position);
            }
        }
        assert(closestNode);
        return closestNode;
    }

    Path* Level::getPath(NPC_Node *start, NPC_Node *end){
        PathFinder<NPC_Node> pathFinder;
        std::vector<NPC_Node*> path;
        pathFinder.setStart(*start);
        pathFinder.setGoal(*end);

        bool result = pathFinder.findPath<Dijkstra>(path, _physicsManager);
        if (result){
            Path* _primaryPath = new Path(&path, end->getPosition());
            return _primaryPath;
        }
        return nullptr;
    }
    Path* Level::getPath(glm::vec3 start, glm::vec3 end){
        PathFinder<NPC_Node> pathFinder;
        std::vector<NPC_Node*> path;
        auto startNode = getNodeNear(start);
        auto endNode = getNodeNear(end);
        pathFinder.setStart(*startNode);
        pathFinder.setGoal(*endNode);

        std::vector<NPC_Node*> *_path = new std::vector<NPC_Node*>;

        auto cachedPath = _cachePaths.find(std::pair<NPC_Node*, NPC_Node*>(startNode, endNode));
        if (cachedPath != _cachePaths.end()){
            return new Path(new std::vector<NPC_Node*>(*cachedPath->second), end);
        }


        //uint32_t startTime = SDL_GetTicks();
        bool result = pathFinder.findPath<Dijkstra>(path, _physicsManager);
        //printf("Time took to path find: %d\n", SDL_GetTicks() - startTime);
        if (result){
            for (auto cNode : path){
                _path->push_back(cNode);
            }
            _cachePaths[std::pair<NPC_Node*, NPC_Node*>(startNode, endNode)] = new std::vector<NPC_Node*>(*_path);

            Path* _primaryPath = new Path(new std::vector<NPC_Node*>(*_path), end);
            return _primaryPath;
        }
        std::vector<NPC_Node*> *_fakePath = new std::vector<NPC_Node*>();
        path.push_back(getNodeNear(end));
        return new Path(_fakePath, end);
    }
} // DNPC