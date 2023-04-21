#pragma once

#include "WOPhysicsTriangleMesh.h"

namespace Aftr {
class WOPhysicsGoalBox : public WOPhysicsTriangleMesh {
   public:
    static WOPhysicsGoalBox* New(Vector scale = Vector(1, 1, 1), MESH_SHADING_TYPE shadingType = MESH_SHADING_TYPE::mstAUTO);
    virtual ~WOPhysicsGoalBox();
    virtual void onCreatePhysics(physx::PxActor* actor) override;

   protected:
    WOPhysicsGoalBox();
    virtual void onCreate(Vector scale = Vector(1, 1, 1), MESH_SHADING_TYPE shadingType = MESH_SHADING_TYPE::mstAUTO);
};
}  // namespace Aftr
