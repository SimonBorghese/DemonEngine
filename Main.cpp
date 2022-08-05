#include <stdlib.h>
#include <stdio.h>

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/ext.hpp>

#include <DemonRender/DR_Shader.h>

#include <DemonGame/Master/Engine.h>
#include <DemonPhysics/DP_CharacterController.h>
#include <DemonRender/DemonLights/DR_DL_BasicLight.h>
#include <DemonGame/Shared/External/DG_EXT_PhysicsCallback.h>
#include <DemonIO/DI_BSPLoader.h>
#include <DemonGame/Master/BSPLoader.h>

#include <math.h>

#define WIDTH 800
#define HEIGHT 600
#define SPEED 90
#define JUMP_HEIGHT 0.3f
#define GRAVITY_MULTIPLIER 10.0f
#define DEFAULT_GRAVITY -9.81f * GRAVITY_MULTIPLIER
float gravity = DEFAULT_GRAVITY;


DemonEngine::Engine *engine;
DemonPhysics::DP_CharacterController *controller;
glm::vec3 FPSFront = glm::vec3(0.0f);

DemonGame::DG_Entity *weaponThing;

void SCPCollideCallback(DemonGame::DG_PhysicsObject* thisObj, DemonGame::DG_Object* other){
    //printf("%s just touched: %s\n",thisObj->getName().c_str(), other->getName().c_str());
    //other->getActor()->setPosition(glm::vec3(0.0f, 20.0f, 0.0f));
    //other->getActor()->applyForce(-engine->getCamera()->getCameraFront() * 2000.0f);
    thisObj->getActor()->applyForce(-engine->getCamera()->getCameraFront() * 2000.0f);
    if (other->type == DemonGame::STATIC){
        printf("The other is static\n");
    }
    else{
        printf("The other is dynamic\n");
        other->physObj->getActor()->applyForce(-engine->getCamera()->getCameraFront() * 2000.0f);
    }
}

//std::vector<glm::vec3> starts;
//unsigned int currentStart = 0;


void keyDownCallback(int scancode){
    auto newBlock = (DemonGame::DG_PhysicsObject*)0;
    switch (scancode){
        case SDL_SCANCODE_E:
            newBlock = engine->createWorldEntity();
            newBlock->createEntityFromMesh("block.obj",
                                           weaponThing->getTransform()->getPosition() +
                                           (engine->getCamera()->getFPSFront() * 3)
                                           );
            newBlock->setContactCallback(SCPCollideCallback);
            newBlock->getActor()->getRealActor()->setMass(50.0f);
            newBlock->getActor()->applyForce(glm::vec3(engine->getCamera()->getCameraFront() * 1000));
            //controller->translate(starts.at(currentStart++) + glm::vec3(0.0f, controller->getHeight(), 0.0f));
            //if (currentStart == starts.size()){
            //    currentStart = 0;
            //}
            break;
        case SDL_SCANCODE_SPACE:
            if (controller->onGround()) {
                gravity = DEFAULT_GRAVITY * -JUMP_HEIGHT;
            }
            break;
        default:
            break;
    }
}

void keyCalback(int scancode){
    //sFPSFront.y = 0.0f;
    switch (scancode){
        case SDL_SCANCODE_W:
            if (!engine->getEvent()->getKey(SDL_SCANCODE_LCTRL)) {
                controller->move(engine->getCamera()->getForward() * engine->getDeltaTime() * SPEED);
            }
            else{
                controller->translate(controller->getPosition() + engine->getCamera()->getForward() * engine->getDeltaTime() * SPEED);
            }
            break;
        case SDL_SCANCODE_S:
            controller->move(engine->getCamera()->getBackward() * engine->getDeltaTime() * SPEED);
            break;
        case SDL_SCANCODE_A:
            controller->move(engine->getCamera()->getLeft() * engine->getDeltaTime() * SPEED);
            break;
        case SDL_SCANCODE_D:
            controller->move(engine->getCamera()->getRight() * engine->getDeltaTime() * SPEED);
            break;
        default:
            break;
    }
}
void bspCallback(DemonEngine::BSP_EntityCreateInfo info){
    //printf("Got: %s\n", info.name);
    //printf("Pos: %f %f %f\n", info.pos.x, info.pos.z, -info.pos.y);
    glm::vec3 realPos = glm::vec3(info.pos.x, info.pos.z, -info.pos.y);
    if (!strcmp(info.name, "info_player_start")){
        //printf("Found player start\n");
        controller->translate(realPos);

        //To-Do: Remove this line
        //engine->createWorldObject()->createEntityFromMesh("block.obj", realPos * (2.0f), glm::vec3(0.0f), glm::vec3(3.0f));
    }
}

int main(void) {
    // Initialize the engine
    engine = new DemonEngine::Engine(WIDTH, HEIGHT);

    // Create the engine elements
    engine->createEngine();

    // Create the FPS controller
    controller = engine->createFPSController(glm::vec3(0.0f), 10.0f, 5.0f);
    controller->translate(glm::vec3(0.0f));


    DemonEngine::BSPLoader bspLoader(engine);
    bspLoader.setBSPCreationCallback(bspCallback);
    bspLoader.loadBSP("notmy4.bsp");
    //engine->createWorldObject()->createEntityFromMesh("bloque.obj", glm::vec3(0.0f, -10.0f, 0.0f));
    

    // Create a light
    auto *light1 = engine->createEasyPointLight(glm::vec3(0.0f, -2.0f, 0.0f), 100.0f, 1.0f);
    //DemonEngine::Engine::DIRECTIONAL_LIGHT_INFO dirLightInfo;
    //dirLightInfo.direction = glm::vec3(0.0f, 10.0f, 0.0f);
    //dirLightInfo.specularAccuracy = 32;
    //dirLightInfo.ambientStrength = 0.5f;
    //dirLightInfo.specularStrength = 0.5f;
    //dirLightInfo.colour = glm::vec3(0.0f);
    //engine->createDirectionalLight(dirLightInfo);

    // Set the key callback functions
    engine->getEvent()->setKeyDownCallback(keyDownCallback);
    engine->getEvent()->setKeyCallback(keyCalback);

    weaponThing = engine->createVisualEntity();
    weaponThing->createEntityFromMesh("zapper.fbx");
    //weaponThing->getTransform()->toggleTransformations();




    // Main game loop

    unsigned int fps = 0;
    unsigned int lastFPSCheck = SDL_GetTicks();
    while (!engine->gameLoop()) {
        light1->getLightInfo()->position = controller->getPosition();
        if (!engine->getEvent()->getKey(SDL_SCANCODE_LCTRL)) {

            weaponThing->getTransform()->setPosition(engine->getCamera()->getPosition() +
            (engine->getCamera()->getCameraFront() * 5) +
            (engine->getCamera()->getCameraRight() * 3) +
            glm::vec3(0.0f, -5.0f, 0.0f));

            weaponThing->getTransform()->setRotation(glm::vec3(glm::radians(-engine->getCamera()->getYRotation()),
                                                               glm::radians((-(engine->getCamera()->getXRotation())) + 90.0f), 0.0f));
            FPSFront = engine->getCamera()->getFPSFront();
            controller->move(glm::vec3(0.0f, gravity * engine->getDeltaTime(), 0.0f));
        }
        else{
            FPSFront = engine->getCamera()->getCameraFront();
        }

        if (controller->onGround() && gravity != DEFAULT_GRAVITY){
            gravity = DEFAULT_GRAVITY;
        }
        else if (gravity > DEFAULT_GRAVITY){
            gravity += DEFAULT_GRAVITY * engine->getDeltaTime();
        }
        light1->getLightInfo()->position = controller->getPosition();

        fps++;
        if (SDL_GetTicks() - lastFPSCheck >= 1000){
            printf("Current FPS: %d\n", fps);
            fps = 0;
            lastFPSCheck = SDL_GetTicks();
        }
    }

    engine->destroyEngine();
    delete engine;

    return 0;
}

/*
DemonEngine::Engine engine(WIDTH, HEIGHT);
DemonGame::DG_RigidEntity *shipEnt;
std::vector<DemonGame::DG_PhysicsObject*> destructionQueue;

void AlienPhysicsCallback(DemonGame::DG_PhysicsObject* thisObj, DemonGame::DG_PhysicsObject* other){
    if (!strcmp(other->getName().c_str(), "bullet")){
        destructionQueue.push_back(thisObj);
    }
}

void keyDownCallback(int scancode){
    if (scancode == SDL_SCANCODE_E){
        auto block = engine.createWorldEntity();
        block->createEntityFromMesh("nblock.obj",
                                        shipEnt->getTransform()->getPosition() +
                                        glm::vec3(5.0f, 0.0f, 0.0f));
        block->setName("bullet");
    }
}

void keyCallback(int scancode){
    switch (scancode){
        case SDL_SCANCODE_A:
            shipEnt->getActor()->translate(glm::vec3(0.0f, 0.0f, -10.0f) * engine.getDeltaTime());
            break;
        case SDL_SCANCODE_D:
            shipEnt->getActor()->translate(glm::vec3(0.0f, 0.0f, 10.0f) * engine.getDeltaTime());
            break;
        default:
            break;
    }
}

void bspCallback(DemonEngine::BSP_EntityCreateInfo info){
    if (!strcmp(info.name, "alien")){
        auto alien = engine.createWorldEntity();
        alien->createEntityFromMesh("nblock.obj", glm::vec3(info.pos.x, info.pos.z, -info.pos.y));
        alien->setContactCallback(AlienPhysicsCallback);
        alien->disableGravity();
    }
}

int main(void){
    engine.createEngine();
    shipEnt = engine.createWorldObject();
    shipEnt->createEntityFromMesh("nblock.obj");

    DemonEngine::BSPLoader bspLoader(&engine);
    bspLoader.setBSPCreationCallback(bspCallback);
    bspLoader.loadBSP("alien.bsp");

    engine.setGravity(glm::vec3(50.0f, 0.0f, 0.0f));


    engine.getEvent()->setKeyDownCallback(keyDownCallback);
    engine.getEvent()->setKeyCallback(keyCallback);
    engine.createEasyPointLight(glm::vec3(0.0f), 100.0f, 1.0f);
    /*
    // Below: Particle Test
    Vertex billiboardVertices[4];
    billiboardVertices[0].iPosition = glm::vec3(-0.5f, -0.5f, -0.0f);
    billiboardVertices[0].iTextCord = glm::vec3(-1.0f, -1.0f, 0.0f);
    billiboardVertices[0].iNormal = glm::vec3(1.0f);

    billiboardVertices[1].iPosition = glm::vec3(0.5f, -0.5f, 0.0f);
    billiboardVertices[1].iTextCord = glm::vec3(1.0f, -1.0f, 0.0f);
    billiboardVertices[1].iNormal = glm::vec3(1.0f);

    billiboardVertices[2].iPosition = glm::vec3(-0.5f, 0.5f, -0.0f);
    billiboardVertices[2].iTextCord = glm::vec3(-1.0f, 1.0f, 0.0f);
    billiboardVertices[2].iNormal = glm::vec3(1.0f);

    billiboardVertices[3].iPosition = glm::vec3(0.5f, 0.5f, 0.0f);
    billiboardVertices[3].iTextCord = glm::vec3(1.0f, 1.0f, 0.0f);
    billiboardVertices[3].iNormal = glm::vec3(1.0f);

    unsigned int indices[6];
    indices[0] = 0;
    indices[1] = 2;
    indices[2] = 3;

    indices[3] = 3;
    indices[4] = 1;
    indices[5] = 0;

    Vertex billiboardVertices[1];
    billiboardVertices[0].iPosition = glm::vec3(-0.5f, -0.5f, -0.0f);
    billiboardVertices[0].iTextCord = glm::vec3(-1.0f, -1.0f, 0.0f);
    billiboardVertices[0].iNormal = glm::vec3(1.0f);

    unsigned int indices[1];
    indices[0] = 0;

    DemonRender::DR_Mesh billiBoardmesh(&billiboardVertices[0], 1, &indices[0], 1);
    billiBoardmesh.type = GL_POINTS;
    billiBoardmesh.createTextureFromSTB("explosion.png", true);
    DemonRender::DR_MeshRenderer billiBoardRenderer;
    billiBoardRenderer.addMesh(&billiBoardmesh);

    DemonRender::DR_Shader billiboardShader;
    billiboardShader.createProgram("DemonShaders/vertex_particle.glsl", "DemonShaders/geo_dot2quad.glsl", "DemonShaders/frag_colourDebug.glsl");

    billiBoardRenderer.setShader(&billiboardShader);


    DemonWorld::DW_Transform objTransform;
    objTransform.setPosition(glm::vec3(5.0f, 5.0f, 5.0f));
    billiBoardRenderer.bindTransform(&objTransform);

    engine.getRenderingManager()->addMeshGroup(&billiBoardRenderer);
    //DemonRender::DR_Mesh billiboard()


    // End particle test

    while (!engine.gameLoop()){
        if (!engine.getEvent()->getKey(SDL_SCANCODE_LCTRL)) {
            engine.getCameraController()->setRotation(glm::vec2(0.0f, -35.0f));
        }
        engine.createFPSController(glm::vec3(5.0f, 5.0f, 0.0f), 2.0f, 1.0f);

        //engine.getCamera()->setPosition(shipEnt->getTransform()->getPosition() + glm::vec3(-10.0f, 10.0f, 0.0f));


        for (unsigned int a = 0; a < destructionQueue.size(); a++){
            //destructionQueue.at(a)->destroyEntity();
            destructionQueue.at(a)->setPosition(glm::vec3(-1000.0f));
            //engine.getWorld()->removeWorldEntityValue(destructionQueue.at(a));
            //delete destructionQueue.at(a);
        }
        destructionQueue.clear();
    }

    engine.destroyEngine();
}
*/