#pragma once

#include "IFaceWOPhysics.h"
#include "PxActor.h"
#include "WOPhysicsAbstract.h"

namespace Aftr {
class WOPhysics : public WOPhysicsAbstract {
   public:
    static WOPhysics* New(physx::PxActor* actor);
    static WOPhysics* New(physx::PxActor* actor, const std::string& modelFileName, Vector scale = Vector(1, 1, 1), MESH_SHADING_TYPE shadingType = MESH_SHADING_TYPE::mstAUTO);
    virtual ~WOPhysics();

    // virtual void onCreatePhysics(physx::PxActor* actor);
    // virtual void updatePoseFromPhysics();
    // virtual void setPosition(const Vector& newXYZ) override;
    // virtual void setPosition(float x, float y, float z) override;
    // virtual void setPose(const Mat4& orientation_w_position) noexcept override;
    // virtual void setDisplayMatrix(const Mat4& dcm) noexcept override;

   protected:
    WOPhysics();
    virtual void onCreate(physx::PxActor* actor);
    virtual void onCreate(physx::PxActor* actor, const std::string& modelFileName, Vector scale = Vector(1, 1, 1), MESH_SHADING_TYPE shadingType = MESH_SHADING_TYPE::mstAUTO);
};
}  // namespace Aftr
