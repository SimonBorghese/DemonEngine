
#include <DemonGame/Master/Engine.h>
#include <DemonGame/Master/BSPLoader.h>
#include <DemonGame/Master/World.h>
#include <DemonGame/Shared/Lua/LuaInterface.h>
#include <DemonPhysics/DP_CharacterController.h>
#include <math.h>
#include <GameClients/Protal/npc_charger.h>

#include <PathFinder.h>
#include <DemonNPC/Level.h>
#include <DemonMacro/DemonBench.h>



DemonEngine::Engine *engine;
DemonEngine::BSPLoader *bspLoader;
DemonEngine::World *world;
DemonPhysics::DP_CharacterController *player;
DG::Lua::LuaInterface luaInterface;
DNPC::Level *_npcWorld;

std::vector<Protal::npc_charger*> chargers;
uint32_t maxChargers = 200.0f;
uint32_t radius = 20.0f;

DG_RigidEntity *follower;


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


std::vector<glm::vec3> lines;

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
#define SHADOW_RES 512
                engine->createEasyPointLight(realPos, distance, intensity)->createShadowBuffer(
                        SHADOW_RES * SHADOW_HELL, SHADOW_RES * SHADOW_HELL);
                engine->createEasyPointLight(realPos, distance, intensity);
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
            case INFO_NODE:
            {
                _npcWorld->addNode(realPos);
            }
                break;
            default:
                break;
        }
    }
}

std::vector<glm::vec3> Epath;
std::vector<glm::vec3> targets;


DGL::Shader* lineShader;
GLuint VAO;
GLuint VBO;
glm::vec3 prevPosition = glm::vec3(-1.0f);

DG_Entity *startMarker;
DG_Entity *endMarker;


glm::vec3 startPos;
glm::vec3 endPos;
int run = 0;
DNPC::Path *levelPath;

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

    _npcWorld = new DNPC::Level(engine->getPhysicsManager());



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

    follower = engine->createWorldObject();
    follower->createEntityFromMesh("173/173.fbx", _npcWorld->getNodeNear(glm::vec3(100.0f))->getPosition(), glm::vec3(0.0f), glm::vec3(0.5f));

    lineShader = new DGL::Shader("DemonShaders/vertex_line.glsl", "DemonShaders/fragment_color.glsl");
    lineShader->createShader();

    glCreateVertexArrays(1, &VAO);

    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * lines.size(), lines.data(), GL_STREAM_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*) (0));
    glEnableVertexAttribArray(0);

    engine->midRenderFunc = [](){
        lineShader->useShader();
        glBindVertexArray(VAO);
        glDrawArrays( GL_LINE_STRIP, 0, lines.size());
    };

    prevPosition = glm::vec3(0.0f);

    startMarker = engine->createVisualEntity();
    endMarker = engine->createVisualEntity();
    startMarker->createEntityFromMesh("block.obj");
    endMarker->createEntityFromMesh("block.obj");

    glm::vec3 position = engine->getCamera()->getPosition();
    glm::vec3 followerPos = follower->getTransform()->getPosition();

    uint32_t start = SDL_GetTicks();
    levelPath = _npcWorld->getPath(position, followerPos);
    printf("Benchmark time: %d\n", SDL_GetTicks() - start);
    assert(levelPath);
    run = 1;
    //startMarker->getTransform()->setPosition(closestFollowerNode->getPosition());
    //endMarker->getTransform()->setPosition(closestPlayerNode->getPosition());

    //std::reverse(targets.begin(), targets.end());
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * lines.size(), lines.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    follower->setName("SCP 173");
}

float currentTime = 0.0f;

int  loop(){

    //for (auto mesh : follower->getMeshRenderer()->getMeshes()){
    //    mesh->_displaySamples = 1;
    //}
    currentTime += engine->getDeltaTime();

    glm::vec3 playerP = glm::vec3(engine->getCamera()->getPosition());
    glm::vec3 origin = follower->getTransform()->getPosition() + glm::vec3(0.0f, 2.0f, 0.0f);
    //printf("Follower is rendered: %d\n", follower->getMeshRenderer()->getRenderStatus());b


    //printf("Current values: %f %f\n", (playerP.z - origin.z), (playerP.x - origin.x));
    if (engine->getEvent()->getKeyDown(SDL_SCANCODE_V)){
        engine->setGameState("noclip", !engine->getGameState("noclip"));
    }


    if (engine->getEvent()->getKeyDown(SDL_SCANCODE_RALT)){
        startPos = engine->getCamera()->getPosition();
        run = 0;
    }
    if (engine->getEvent()->getKeyDown(SDL_SCANCODE_RCTRL)){
        endPos = engine->getCamera()->getPosition();
        run = 0;
    }
    if (engine->getCamera()->getPosition() != prevPosition || prevPosition == glm::vec3(-1.0f)){
        prevPosition = engine->getCamera()->getPosition();
        glm::vec3 position = engine->getCamera()->getPosition();
        glm::vec3 followerPos = follower->getTransform()->getPosition();

        uint32_t start = SDL_GetTicks();
        levelPath = _npcWorld->getPath(position, followerPos);
        printf("Benchmark time: %d\n", SDL_GetTicks() - start);
        assert(levelPath);
        run = 1;
        //startMarker->getTransform()->setPosition(closestFollowerNode->getPosition());
        //endMarker->getTransform()->setPosition(closestPlayerNode->getPosition());

        //std::reverse(targets.begin(), targets.end());
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec3) * lines.size(), lines.data());
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
    levelPath->setStartPosition(follower->getTransform()->getPosition());

    glm::vec3 viewForward = engine->getCamera()->getPosition() - follower->getTransform()->getPosition();
    viewForward = glm::normalize(viewForward);
    //float dot = glm::dot(viewForward, engine->getCamera()->getCameraFront());
    //printf("Dot product: %f\n", dot);

    glm::vec3 moveTarget = glm::normalize(levelPath->getNextTarget() - follower->getTransform()->getPosition()) *
                           (float) engine->getDeltaTime() * 30.0f;
    if (run && !follower->getMeshRenderer()->getRenderStatus()) {
        float roty = glm::atan((playerP.z - origin.z) / (playerP.x - origin.x));

        if ((playerP.x - origin.x) > 0){
            roty = roty - glm::radians(180.0f);
        }
        roty = (-roty - glm::radians(90.0f));
        follower->getActor()->setRotation(glm::quat(glm::vec3(0.0f,roty,0.0f)));

        if (glm::distance(follower->getTransform()->getPosition(), levelPath->getNextTarget()) > 1.0f) {
            follower->getActor()->translate(moveTarget);
        } else {
            levelPath->advanceTarget();
        }

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

    return !engine->gameLoop(fmin(engine->getDeltaTime(), 0.016f));
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