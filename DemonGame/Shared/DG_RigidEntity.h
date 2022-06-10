//
// Created by simon on 6/7/22.
//

#ifndef DEMONENGINE_DG_RIGIDENTITY_H
#define DEMONENGINE_DG_RIGIDENTITY_H
#include "DG_Entity.h"
#include <DemonPhysics/DP_RigidActor.h>
#include <DemonRender/DR_RenderManager.h>
#include <DemonPhysics/DP_PhysicsManager.h>

namespace DemonGame {

    class DG_RigidEntity : public DG_Entity{
    public:
        DG_RigidEntity(DemonRender::DR_RenderManager *targetRender, DemonRender::DR_Shader *targetShader, DemonPhysics::DP_PhysicsManager *targetManager);

    protected:
        DemonPhysics::DP_PhysicsManager *physicsManager;
    };

} // DemonGame

#endif //DEMONENGINE_DG_RIGIDENTITY_H
