#pragma once

#include "IFaceWOPhysics.h"
#include "PxActor.h"
#include "WOPhysicsAbstract.h"

namespace Aftr {
class WOPhysics : public WOPhysicsAbstract {
   public:
    static WOPhysics* New(physx::PxActor* actor);
    static WOPhysics* New(physx::PxActor* actor, const std::string& modelFileName, Vector scale = Vector(1, 1, 1), MESH_SHADING_TYPE shadingType = MESH_SHADING_TYPE::mstFLAT);
    virtual ~WOPhysics();

   protected:
    WOPhysics();
    virtual void onCreate(physx::PxActor* actor);
    virtual void onCreate(physx::PxActor* actor, const std::string& modelFileName, Vector scale = Vector(1, 1, 1), MESH_SHADING_TYPE shadingType = MESH_SHADING_TYPE::mstFLAT);
};
}  // namespace Aftr
