#define GET_SECONDS() (SDL_GetTicks() / 1000.0f)

#include <DemonGame/Master/Engine.h>
#include <DemonGame/Master/BSPLoader.h>
#include <DemonGame/Master/World.h>
#include <DemonGame/Shared/Lua/LuaInterface.h>
#include <DemonPhysics/DP_CharacterController.h>
#include <cmath>
#include <GameClients/Protal/npc_173.h>
#include <GameClients/Protal/cl_player.h>
#include <GameClients/Protal/npc_charger.h>
#include <GameClients/Protal/npc_knight.h>
#include <GameClients/Protal/door_bars.h>

#include <PathFinder.h>
#include <DemonNPC/Level.h>
#include <DemonMacro/DemonBench.h>



DemonEngine::Engine *engine;
DemonEngine::BSPLoader *bspLoader;
DemonEngine::World *world;
DG::Lua::LuaInterface luaInterface;
DNPC::Level *npcWorld;


float health = 100.0f;


Protal::cl_player *player;

float destroyWorld = 0;
std::string newWorld;
int totalFrames = 0;

DGL::Light *spotLight;

void bspCallback(DemonEngine::BSP_EntityCreateInfo _info) {
    enum BSP_ENTITIES {
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
        INFO_SCRIPTED_PROP,
        INFO_NODE,
        NPC_173,
        TRIGGER_LEVELCHANGE,
        NPC_CHARGER,
        DOOR_BARS,
        INFO_SPOT_LIGHT
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
    INSERT("info_node", INFO_NODE);
    INSERT("npc_173", NPC_173);
    INSERT("trigger_levelChange", TRIGGER_LEVELCHANGE);
    INSERT("npc_charger", NPC_CHARGER);
    INSERT("door_bars", DOOR_BARS);
    INSERT("info_spot_light", INFO_SPOT_LIGHT);

    glm::vec3 realPos = glm::vec3(_info.pos.x, _info.pos.z, -_info.pos.y);

    auto lookingEntity = entityList.find(_info.name);
    //printf("Ent Name: %s and %d\n", info.name, info.entityNumber);

    if (lookingEntity != entityList.end()) {
        switch (lookingEntity->second) {
            case INFO_PLAYER_START: {
                player = new Protal::cl_player(glm::vec3(0.0f, 100.0f, 0.0f), 6.0f, 1.0f, engine);
                player->init();
                engine->addClient(player);
                player->getController()->translate(realPos);
            }
                break;
            case INFO_BRUSHPROP: {
                if (_info.brushMeshes) {
                    auto newProp = engine->createWorldEntity();
                    auto objMass = CBSP_getKeyFromEntity(_info.currentEntity, "mass");
                    float mass = 0.0f;
                    if (strcmp(objMass, CBSP_getKeyFromEntity_FAILURE) != 0) {
                        mass = (float) strtod(objMass, nullptr);
                    }

                    newProp->createEntityFromExistingMesh(_info.brushMeshes, _info.numBrushMesh, _info.origin,
                                                          glm::vec3(0.0f, glm::radians(_info.angle), 0.0f));
                    newProp->setName("UNNAMED BUSH PROP");
                    if (mass > 0.0f) {
                        //newProp->setMass(mass);
                    }
                    auto *newMat = new DemonPhysics::DP_PhysicsMaterial(0.8f, 0.5f,
                                                                        0.8f);
                    newMat->createMaterial(engine->getPhysicsManager()->getPhysics());
                    newProp->setMaterial(newMat);
                }
            }
                break;
            case INFO_POINT_LIGHT: {
                // -2 is down, -1 is up, all other angles are on y axis
                auto distance = (float) strtod(CBSP_getKeyFromEntity(_info.currentEntity, "distance"), nullptr);
                auto intensity =
                        (float) strtol(CBSP_getKeyFromEntity(_info.currentEntity, "intensity"), nullptr, 10) / 100.0f;

#define SHADOW_HELL 4.0f
#define SHADOW_RES 512
                engine->createEasyPointLight(realPos, distance, intensity)->createShadowBuffer(
                        SHADOW_RES * SHADOW_HELL, SHADOW_RES * SHADOW_HELL);
                //engine->createEasyPointLight(realPos, distance, intensity);
            }
                break;

            case INFO_SPOT_LIGHT: {

                auto distance = (float) strtod(CBSP_getKeyFromEntity(_info.currentEntity, "distance"), nullptr);
                auto intensity =
                        (float) strtol(CBSP_getKeyFromEntity(_info.currentEntity, "intensity"), nullptr, 10) / 100.0f;
                auto angle = (float) strtod(CBSP_getKeyFromEntity(_info.currentEntity, "angle"), nullptr);

                float y = 0.0f;
                if (angle <= -2.0f) {
                    y = -90.0f;
                } else if (angle <= -1.0f) {
                    y = 90.0f;
                }
                float x = angle;

                glm::vec3 cameraFront = glm::vec3(0.0f);
                cameraFront.x = cos(glm::radians(x)) * cos(glm::radians(y));
                cameraFront.y = sin(glm::radians(y));
                cameraFront.z = sin(glm::radians(x)) * cos(glm::radians(y));
                cameraFront = glm::normalize(cameraFront);


                spotLight = engine->createEasySpotLight(realPos, cameraFront, glm::radians(45.0f), distance, intensity);

            }
                break;
            case INFO_SCRIPTED_PROP: {
                /*
                const char *script = CBSP_getKeyFromEntity(_info.currentEntity, "script");
                if (strcmp(script, CBSP_getKeyFromEntity_FAILURE) != 0){
                    luaInterface.initFile(script);
                    luaInterface.callFunction("onInit");
                    //assert(0);
                } else{
                    assert(0);
                }
                 */
            }
                break;
            case INFO_NODE:
            {
                npcWorld->addNode(realPos);
            }
                break;
            case NPC_173:
            {
                auto newNPC = new Protal::npc_173("173/173.fbx", realPos, npcWorld, engine);
                newNPC->init();
                engine->addClient(newNPC);
            }
                break;
            case NPC_CHARGER:
            {
                auto charger = new Protal::npc_charger(engine, realPos, new float(100.0f));
                charger->init();
                engine->addClient(charger);
            }
                break;
            case TRIGGER_LEVELCHANGE:
            {
                destroyWorld = 0.0f;
                auto newTrigger = engine->createTrigger();
                newTrigger->createEntityFromExistingMesh(_info.brushMeshes, _info.numBrushMesh, _info.origin);
                newTrigger->toggleRender(0);
                std::string newLevel = CBSP_getKeyFromEntity(_info.currentEntity, "level");
                newTrigger->setCallback([newLevel](DG_Object *, bool isPlayer) {
                    // In order to prevent premature activation, 5 frames must render before it can be triggered
                    if (isPlayer && totalFrames > 5) {
                        destroyWorld += 1.0;
                        newWorld = newLevel;
                    }
                });
            }
                break;
            case INFO_KEY: {
                auto newTrigger = engine->createTrigger();
                newTrigger->createEntityFromMesh("block", realPos, glm::vec3(0.0f), glm::vec3(2.0f));
                newTrigger->toggleRender(0);
                std::string newKey = CBSP_getKeyFromEntity(_info.currentEntity, "key");
                int *activated = new int(0);
                newTrigger->setCallback([newKey, activated](DG_Object *, bool isPlayer) {
                    // In order to prevent premature activation, 5 frames must render before it can be triggered
                    if (!(*activated) && isPlayer && totalFrames > 5) {
                        ((Protal::door_bars *) engine->getClient(fmt::format("{}_door", newKey)))->openDoor();
                        *activated = 1;
                    }
                });
            }
                break;
            case DOOR_BARS: {
                auto theBars = engine->createWorldObject();
                theBars->createEntityFromExistingMesh(_info.brushMeshes, _info.numBrushMesh, _info.origin);
                std::string newKey = CBSP_getKeyFromEntity(_info.currentEntity, "key");
                Protal::door_bars *_door = new Protal::door_bars(engine, theBars, 5.0f);
                engine->addClient(fmt::format("{}_door", newKey), _door);
                /*
                theBars->setUpdateFunc([newKey](DG_Entity *ent) {
                    if (engine->getGameState(fmt::format("{}_active", newKey))) {
                        DG_RigidEntity *realEnt = (DG_RigidEntity *) ent;
                        realEnt->getActor()->translate(glm::vec3(0.0f, 5.0f, 0.0f) * (float) engine->getDeltaTime());
                    }
                });
                 */

            }
                break;
            default:
                break;
        }
    }
}

void init() {
    // Init Engine
    engine = new DemonEngine::Engine(1600, 900);
    engine->createEngine();
    engine->getWindow()->setMouseGrab(-1);

    // Init BSP Loader
    bspLoader = new DemonEngine::BSPLoader(engine);

    // Create the player's character controller
    //player = engine->createFPSController(glm::vec3(0.0f, 100.0f, 0.0f), 6.0f, 1.0f);

    //luaInterface.registerFunction("createBlockAtPlayer", &Protal::createBlockAtPlayer);
    luaInterface.registerFunction("createBlockAtPlayer", [](lua_State *)mutable {
        engine->createWorldEntity()->createEntityFromMesh("block.obj", engine->getCamera()->getPosition());
        return 0;
    });

    npcWorld = new DNPC::Level(engine->getPhysicsManager());



    // Load the BSP map
    bspLoader->setBSPCreationCallback([](DemonEngine::BSP_EntityCreateInfo _info) {
        bspCallback(_info);
    });
    bspLoader->loadBSP("level99");

    //engine->addClient(new Protal::npc_knight(glm::vec3(0.0f, 5.0f, 0.0f), nullptr, engine));



}

int  loop(){
    //printf("State: %d\n", engine->getGameState("secret_key_active"));

    if (destroyWorld >= 1.0f){
        engine->destroyScene();
        delete player;
        bspLoader->loadBSP(newWorld.c_str());
        destroyWorld = 0.0f;
        totalFrames = 0;
    }

    if (engine->getEvent()->getKeyDown(SDL_SCANCODE_V)){
        engine->setGameState("noclip", !engine->getGameState("noclip"));
    }

    if (engine->getEvent()->getKeyDown(SDL_SCANCODE_M)){
        engine->getWindow()->setMouseGrab(-1);
        engine->getEvent()->toggleMouseMovements();
    }

    ImGui::Begin("Debug");
    ImGui::Text("FPS: %f\n", ImGui::GetIO().Framerate);
    ImGui::Text("Health: %f\n", health);
    ImGui::Text("Frames: %d\n", totalFrames);
    ImGui::End();
    totalFrames++;
    return !engine->gameLoop((float) fmin(engine->getDeltaTime(), 0.016f));
}
void close(){
    //charger->destroy();
    engine->destroyEngine();
}

int main(){
    init();
    while (loop()){}
    close();
}