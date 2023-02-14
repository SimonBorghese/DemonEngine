
#include <DemonGame/Master/Engine.h>
#include <DemonGame/Master/BSPLoader.h>
#include <DemonGame/Master/World.h>
#include <DemonGame/Shared/Lua/LuaInterface.h>
#include <DemonPhysics/DP_CharacterController.h>
#include <cmath>
#include <GameClients/Protal/npc_173.h>

#include <PathFinder.h>
#include <DemonNPC/Level.h>
#include <DemonMacro/DemonBench.h>



DemonEngine::Engine *engine;
DemonEngine::BSPLoader *bspLoader;
DemonEngine::World *world;
DemonPhysics::DP_CharacterController *player;
DG::Lua::LuaInterface luaInterface;
DNPC::Level *npcWorld;


float health = 100.0f;

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
        default:
            break;

    }
}

void keyDownCallback(int SCANCODE){
    switch (SCANCODE){
        case SDL_SCANCODE_E:
            auto projectile = engine->createWorldEntity();
            projectile->createEntityFromMesh("block.obj", player->getPosition() + (engine->getCamera()->getCameraFront() * 5.0f));
            projectile->setMass(100.0f);
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
        INFO_SCRIPTED_PROP,
        INFO_NODE
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
            case INFO_POINT_LIGHT:
            {
                
                auto distance = (float) strtod(CBSP_getKeyFromEntity(_info.currentEntity, "distance"), nullptr);
                auto intensity = (float) strtol(CBSP_getKeyFromEntity(_info.currentEntity, "intensity"), nullptr, 10) / 100.0f;

#define SHADOW_HELL 1.0f
#define SHADOW_RES 512
                engine->createEasyPointLight(realPos, distance, intensity)->createShadowBuffer(
                        SHADOW_RES * SHADOW_HELL, SHADOW_RES * SHADOW_HELL);
                engine->createEasyPointLight(realPos, distance, intensity);
            }
                break;
            case INFO_SCRIPTED_PROP:
            {
                const char *script = CBSP_getKeyFromEntity(_info.currentEntity, "script");
                if (strcmp(script, CBSP_getKeyFromEntity_FAILURE) != 0){
                    luaInterface.initFile(script);
                    luaInterface.callFunction("onInit");
                    //assert(0);
                } else{
                    assert(0);
                }
            }
                break;
            case INFO_NODE:
            {
                npcWorld->addNode(realPos);
            }
                break;
            default:
                break;
        }
    }
}


Protal::npc_173 *scp;

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
        engine->createWorldEntity()->createEntityFromMesh("block.obj", engine->getCamera()->getPosition());
        return 0;
    });

    npcWorld = new DNPC::Level(engine->getPhysicsManager());



    // Load the BSP map
    bspLoader->setBSPCreationCallback([](DemonEngine::BSP_EntityCreateInfo _info){
        bspCallback(_info);
    });
    bspLoader->loadBSP("worlds/maze");



    // Add the event key callback
    engine->getEvent()->addKeyCallback([](int scancode){
        keyCallback(scancode);
    });
    engine->getEvent()->addKeyDownCallback([](int scancode){
        keyDownCallback(scancode);
    });


    scp = new Protal::npc_173("173/173.fbx", npcWorld->getNodeNear(glm::vec3(100.0f))->getPosition(), npcWorld, engine);
    scp->init();
    engine->addClient(scp);

}

double currentTime = 0.0f;

int  loop(){
    currentTime += engine->getDeltaTime();

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
    ImGui::End();

    // Apply gravity to player
    if (!engine->getGameState("noclip")) {
        player->move(glm::vec3(0.0f, -9.81f * engine->getDeltaTime(), 0.0f));
    }

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