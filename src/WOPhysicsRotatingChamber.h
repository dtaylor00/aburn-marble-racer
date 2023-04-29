#pragma once

#include "WOPhysicsTriangleMesh.h"

namespace Aftr {
class WOPhysicsRotatingChamber : public WOPhysicsTriangleMesh {
   public:
    static WOPhysicsRotatingChamber* New(Vector scale = Vector(1, 1, 1), MESH_SHADING_TYPE shadingType = MESH_SHADING_TYPE::mstAUTO, physx::PxActorType::Enum actorType = physx::PxActorType::eRIGID_STATIC);
    virtual ~WOPhysicsRotatingChamber();

   protected:
    WOPhysicsRotatingChamber();
    virtual void onCreate(Vector scale, MESH_SHADING_TYPE shadingType, physx::PxActorType::Enum actorType);
    virtual void setPose(const Mat4& orientation_w_position) noexcept override;
    virtual void setDisplayMatrix(const Mat4& dcm) noexcept override;

   private:
    bool jointCreated = false;
};
}  // namespace Aftr
