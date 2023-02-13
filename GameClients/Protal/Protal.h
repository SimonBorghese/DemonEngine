//
// Created by simon on 1/20/23.
//

#ifndef DEMONENGINE_PROTAL_H
#define DEMONENGINE_PROTAL_H
#include <GameClients/Game.h>
#include <DemonGame/Shared/Lua/LuaInterface.h>
#include <math.h>



namespace Protal {

    class Protal : public Game{
    public:
        Protal() = default;
        void keyCallback(int SCANCODE){
            switch (SCANCODE){
                case SDL_SCANCODE_W:
                    player->move(engine->getCamera()->getFPSFront());
                    break;
                case SDL_SCANCODE_S:
                    player->move(-engine->getCamera()->getFPSFront());
                    break;
                case SDL_SCANCODE_A:
                    player->move(-engine->getCamera()->getCameraRight());
                    break;
                case SDL_SCANCODE_D:
                    player->move(engine->getCamera()->getCameraRight());
                    break;
                case SDL_SCANCODE_E:
                    auto projectile = engine->createWorldEntity();
                    projectile->createEntityFromMesh("block.obj", player->getPosition() + (engine->getCamera()->getCameraFront() * 5.0f));
                    projectile->setMass(1.0f);
                    projectile->getActor()->applyForce(engine->getCamera()->getCameraFront() * 5000.0f);
                    break;
            }
        }

        void bspCallback(DemonEngine::BSP_EntityCreateInfo _info){
            enum BSP_ENTITIES{
                INFO_PLAYER_START,
                INFO_POINT_LIGHT,
                INFO_DIRECTIONAL_LIGHT,
                INFO_DOOR,
                INFO_DOOR_TARGET,
                INFO_KEY,
                PHYS_PROP,
                INFO_BRUSHDOOR,
                INFO_BRUSHDOOR_TARGET,
                INFO_BRUSHPROP,
                INFO_SCRIPTED_PROP
            };

#define INSERT(name, ent) entityList.insert(std::pair<std::string, BSP_ENTITIES>(name, ent))
            std::map<std::string, BSP_ENTITIES> entityList{};
            INSERT("info_player_start", INFO_PLAYER_START);
            INSERT("info_point_light", INFO_POINT_LIGHT);
            INSERT("info_directional_light", INFO_DIRECTIONAL_LIGHT);
            INSERT("info_door", INFO_DOOR);
            INSERT("info_door_target", INFO_DOOR_TARGET);
            INSERT("info_key", INFO_KEY);
            INSERT("phys_prop", PHYS_PROP);
            INSERT("info_brushdoor", INFO_BRUSHDOOR);
            INSERT("info_brushdoor_target", INFO_BRUSHDOOR_TARGET);
            INSERT("info_brushprop", INFO_BRUSHPROP);
            INSERT("info_scripted_prop", INFO_SCRIPTED_PROP);

            glm::vec3 realPos = glm::vec3(_info.pos.x, _info.pos.z, -_info.pos.y);

            auto lookingEntity = entityList.find(_info.name);
            //printf("Ent Name: %s and %d\n", info.name, info.entityNumber);

            if (lookingEntity != entityList.end()){
                switch (lookingEntity->second){
                    case INFO_PLAYER_START:
                    {
                        player->translate(realPos);
                    }
                        break;
                    case INFO_BRUSHPROP:
                    {
                        if (_info.brushMeshes) {
                            auto newProp = engine->createWorldEntity();
                            auto objMass = CBSP_getKeyFromEntity(_info.currentEntity, "mass");
                            float mass = 0.0f;
                            if (strcmp(objMass, CBSP_getKeyFromEntity_FAILURE)) {
                                mass = atof(objMass);
                            }

                            newProp->createEntityFromExistingMesh(_info.brushMeshes, _info.numBrushMesh, _info.origin,
                                                                  glm::vec3(0.0f, glm::radians(_info.angle), 0.0f));
                            if (mass > 0.0f) {
                                //newProp->setMass(mass);
                            }
                            DemonPhysics::DP_PhysicsMaterial *newMat = new DemonPhysics::DP_PhysicsMaterial(0.8f, 0.5f,
                                                                                                            0.8f);
                            newMat->createMaterial(engine->getPhysicsManager()->getPhysics());;
                            newProp->setMaterial(newMat);
                        }
                    }
                        break;
                    case INFO_POINT_LIGHT:
                    {
                        float distance = 0.0f;
                        distance = atof(CBSP_getKeyFromEntity(_info.currentEntity, "distance")) * 1.0f;
                        float intensity = 0.0f;
                        intensity = atoi(CBSP_getKeyFromEntity(_info.currentEntity, "intensity")) / 100.0f;

#define SHADOW_HELL 1.0f
#define SHADOW_RES 1000
                        engine->createEasyPointLight(realPos, distance, intensity)->createShadowBuffer(
                                SHADOW_RES * SHADOW_HELL, SHADOW_RES * SHADOW_HELL);
                    }
                        break;
                    case INFO_SCRIPTED_PROP:
                    {
                        const char *script = CBSP_getKeyFromEntity(_info.currentEntity, "script");
                        if (strcmp(script, CBSP_getKeyFromEntity_FAILURE)){
                            luaInterface.initFile(script);
                            luaInterface.callFunction("onInit");
                            //assert(0);
                        } else{
                            assert(0);
                        }
                    }
                        break;
                    default:
                        break;
                }
            }
        }

        void init(){
            // Init Engine
            engine = new DemonEngine::Engine(1600, 900);
            engine->createEngine();
            engine->getWindow()->setMouseGrab(-1);

            // Init BSP Loader
            bspLoader = new DemonEngine::BSPLoader(engine);

            // Create the player's character controller
            player = engine->createFPSController(glm::vec3(0.0f, 100.0f, 0.0f), 6.0f, 1.0f);


            //luaInterface.registerFunction("createBlockAtPlayer", &Protal::createBlockAtPlayer);
            luaInterface.registerFunction("createBlockAtPlayer", [](lua_State *)mutable {
                return 1;
            });

            // Load the BSP map
            bspLoader->setBSPCreationCallback([this](DemonEngine::BSP_EntityCreateInfo _info){
                bspCallback(_info);
            });
            bspLoader->loadBSP("levels/finalOffice");

            // Add the event key callback
            engine->getEvent()->addKeyCallback([this](int scancode){
                keyCallback(scancode);
            });

        }
        int  loop(){
            player->move(glm::vec3(0.0f, -9.81f * engine->getDeltaTime(), 0.0f));

            if (engine->getEvent()->getKeyDown(SDL_SCANCODE_M)){
                engine->getWindow()->setMouseGrab(-1);
                engine->getEvent()->toggleMouseMovements();
            }

            ImGui::Begin("Debug");
            ImGui::Text("FPS: %f\n", ImGui::GetIO().Framerate);
            ImGui::End();

            return !engine->gameLoop(fmin(engine->getDeltaTime(), 0.016f));
        }
        void close(){
            engine->destroyEngine();
        }
    private:
        DemonPhysics::DP_CharacterController *player;
        DG::Lua::LuaInterface luaInterface;
    };

} // Protal


#endif //DEMONENGINE_PROTAL_H
