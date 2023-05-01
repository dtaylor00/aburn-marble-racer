#pragma once

#include "WOPhysicsTrack.h"

namespace Aftr {
class WOPhysicsGoalBox : public WOPhysicsTrack {
   public:
    static WOPhysicsGoalBox* New(Vector scale = Vector(1, 1, 1), MESH_SHADING_TYPE shadingType = MESH_SHADING_TYPE::mstAUTO, physx::PxActorType::Enum actorType = physx::PxActorType::eRIGID_STATIC);
    virtual ~WOPhysicsGoalBox();
    virtual void onCreatePhysics(physx::PxActor* actor) override;
    virtual WO* clone() override;
    virtual WO* clone(Mat4 pose, Vector scale) override;

   protected:
    WOPhysicsGoalBox();
    virtual void onCreate(Vector scale, MESH_SHADING_TYPE shadingType, physx::PxActorType::Enum actorType);
};
}  // namespace Aftr
