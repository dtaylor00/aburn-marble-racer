#pragma once

#include "WOPhysicsTrack.h"

namespace Aftr {
class WOPhysicsRotatingChamber : public WOPhysicsTrack {
   public:
    static WOPhysicsRotatingChamber* New(Vector scale = Vector(1, 1, 1), MESH_SHADING_TYPE shadingType = MESH_SHADING_TYPE::mstAUTO, physx::PxActorType::Enum actorType = physx::PxActorType::eRIGID_STATIC);
    virtual ~WOPhysicsRotatingChamber();
    virtual WO* clone() override;
    virtual WO* clone(Mat4 pose, Vector scale) override;

   protected:
    WOPhysicsRotatingChamber();
    virtual void onCreate(Vector scale, MESH_SHADING_TYPE shadingType, physx::PxActorType::Enum actorType);
    virtual void setPose(const Mat4& orientation_w_position) noexcept override;
    virtual void setDisplayMatrix(const Mat4& dcm) noexcept override;

   private:
    bool jointCreated = false;
};
}  // namespace Aftr
