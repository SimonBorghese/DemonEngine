//
// Created by simon on 8/5/22.
//

#ifndef DEMONENGINE_B_PHYSUSERDATA_H
#define DEMONENGINE_B_PHYSUSERDATA_H
#include <string>
#include <functional>


namespace DemonGame{
    class DG_PhysicsObject;
    class DG_RigidEntity;


    // This should be used to quickly determine what type a reference points to
    // Static could either be a DG_RigidEntity or DP_RIGID_OBJ_DESC
    // Dynamic could either be DG_PhysicsObject or DP_PHYSICS_OBJ_DESC
    // Trigger could either be a DG_Trigger or DP_TRIGGER_OBJ_DESC
    enum DG_OBJ_TYPE{
        STATIC,
        DYNAMIC,
        TRIGGER,
        CHARACTER
    };

    // This is a general struct for objects
    // If type = STATIC then the reference is to a DG_RigidEntity
    // If type = DYNAMIC then the reference is to a DG_PhysicsObject
    // Always assume that the other reference is null
    typedef struct{
        DG_OBJ_TYPE type;
        DG_PhysicsObject *physObj;
        DG_RigidEntity *rigidObj;
    } DG_Object;
}
using namespace DemonGame;

namespace DemonBase{
    namespace DemonUserData{

        // All objects should have this in their user data's
        // The magicString is used to verify the struct
        // The name is the name of the object
        // The struct Reference points to the specific struct for the object
        // If type = STATIC then the structReference points to a DP_RIGID_OBJ_DESC
        // If type = DYNAMIC then the structReference points to a DP_PHYSICS_OBJ_DESC
        // If type = TRIGGER then the structReference points to a DP_TRIGGER_OBJ_DESC
        // If type = CHARACTER then the structReference points to a DP_CHARACTER_OBJ_DESC
        typedef struct{
            char magicString[5]; // IOBJ
            std::string name;
            DG_OBJ_TYPE type;
            void *originalObject; // Pointing to a GameClient, mostly useless
            void *structReference;
        } generalStruct;

        // All DG_PhysicsObjects should have this struct even if they don't have contact functions
        // The magic string is used to verify the struct
        // The onContact function is called when a physics object collides with another object
        // Reference points to the original object
        typedef struct{
            char magicString[5]; // IOBJ
            std::function<void(DemonGame::DG_PhysicsObject*, DemonGame::DG_Object*)> onContact;
            DemonGame::DG_PhysicsObject *reference;
        } DP_PHYSICS_OBJ_DESC;

        // All DG_RigidEntities should have this struct
        // The magic string is used to verify the struct
        // Since less data is needed for static objects, all this struct does is point to the original entity
        typedef struct{
            char magicString[5]; // IOBJ
            DemonGame::DG_RigidEntity *reference;
        } DP_RIGID_OBJ_DESC;

        typedef struct{
            char magicString[5]; // IOBJ
            std::function<void(DemonGame::DG_Object*, bool)> triggerActivate; // function to run on trigger active, bool if character
        } DP_TRIGGER_OBJ_DESC;

        typedef struct{
            char magicString[5]; // IOBJ
            std::function<void(DemonGame::DG_Object*)> characterCallback; // Characters don't have proper rigid bodies
            // so no originating object
        } DP_CHARACTER_OBJ_DESC;
    }
}

#endif //DEMONENGINE_B_PHYSUSERDATA_H
