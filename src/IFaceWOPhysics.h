#pragma once
#include "AftrConfig.h"
#include "IFace.h"
#include "PxActor.h"

namespace Aftr {

class PhysicsEnginePhysX;

class IFaceWOPhysics : public virtual IFace {
   public:
    IFaceWOPhysics(WO* wo);
    virtual ~IFaceWOPhysics();

    physx::PxActor* getActor();
    void setActor(physx::PxActor* actor);
    virtual void onCreatePhysics(physx::PxActor* actor) = 0;
    virtual void updatePoseFromPhysics() = 0;
    virtual void updatePhysicsFromPose() = 0;

   protected:
    physx::PxActor* actor;
};
}  // namespace Aftr