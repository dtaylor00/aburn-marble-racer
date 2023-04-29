#pragma once

#include "IFaceWOPhysics.h"
#include "WOPhysicsAbstract.h"

namespace Aftr {
class WOPhysicsTriangleMesh : public WOPhysicsAbstract {
   public:
    // actor type can either be eRIGID_STATIC for a static body or eRIGID_DYNAMIC for a _kinematic_ body (not a dynamic body). This is due to physx triangle mesh shape's limitations
    static WOPhysicsTriangleMesh* New(const std::string& modelFileName, Vector scale = Vector(1, 1, 1), MESH_SHADING_TYPE shadingType = MESH_SHADING_TYPE::mstAUTO, physx::PxActorType::Enum actorType = physx::PxActorType::eRIGID_STATIC);
    virtual ~WOPhysicsTriangleMesh();

   protected:
    WOPhysicsTriangleMesh();
    virtual void onCreate(const std::string& modelFileName, Vector scale, MESH_SHADING_TYPE shadingType, physx::PxActorType::Enum actorType);
};
}  // namespace Aftr
