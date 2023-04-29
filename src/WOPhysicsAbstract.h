#pragma once

#include "IFaceWOPhysics.h"
#include "PxActor.h"
#include "WO.h"

namespace Aftr {
class WOPhysicsAbstract : public WO, public IFaceWOPhysics {
   public:
    virtual ~WOPhysicsAbstract();

    virtual void onCreatePhysics(physx::PxActor* actor);
    virtual void updatePoseFromPhysics();
    virtual void updatePhysicsFromPose();
    virtual void setPosition(const Vector& newXYZ) override;
    virtual void setPosition(float x, float y, float z) override;
    virtual void setPose(const Mat4& orientation_w_position) noexcept override;
    virtual void setDisplayMatrix(const Mat4& dcm) noexcept override;
    virtual void moveRelative(const Vector& dXdYdZ) override;

   protected:
    WOPhysicsAbstract();
};
}  // namespace Aftr
