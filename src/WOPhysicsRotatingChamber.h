#pragma once

#include "WOPhysicsTriangleMesh.h"

namespace Aftr {
class WOPhysicsRotatingChamber : public WOPhysicsTriangleMesh {
   public:
    static WOPhysicsRotatingChamber* New(Vector scale = Vector(1, 1, 1), MESH_SHADING_TYPE shadingType = MESH_SHADING_TYPE::mstAUTO, physx::PxActorType::Enum actorType = physx::PxActorType::eRIGID_STATIC);
    virtual ~WOPhysicsRotatingChamber();
    virtual void onCreatePhysics(physx::PxActor* actor) override;
    virtual void createJoint();

   protected:
    WOPhysicsRotatingChamber();
    virtual void onCreate(Vector scale, MESH_SHADING_TYPE shadingType, physx::PxActorType::Enum actorType);

   private:
    bool jointCreated = false;
};
}  // namespace Aftr
