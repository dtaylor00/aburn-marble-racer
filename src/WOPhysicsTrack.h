#pragma once

#include "IFaceWOPhysics.h"
#include "IFaceWOTrack.h"
#include "WOPhysicsTriangleMesh.h"

namespace Aftr {
class WOPhysicsTrack : public WOPhysicsTriangleMesh, public IFaceWOTrack {
   public:
    // actor type can either be eRIGID_STATIC for a static body or eRIGID_DYNAMIC for a _kinematic_ body (not a dynamic body). This is due to physx triangle mesh shape's limitations
    static WOPhysicsTrack* New(const std::string& trackName, const std::string& modelFileName, Vector scale = Vector(1, 1, 1), MESH_SHADING_TYPE shadingType = MESH_SHADING_TYPE::mstAUTO, physx::PxActorType::Enum actorType = physx::PxActorType::eRIGID_STATIC);
    virtual ~WOPhysicsTrack();
    virtual WO* clone() override;
    virtual WO* clone(Mat4 pose, Vector scale) override;
    virtual void removeAllActors() override;

   protected:
    WOPhysicsTrack();
    virtual void onCreate(const std::string& trackName, const std::string& modelFileName, Vector scale, MESH_SHADING_TYPE shadingType, physx::PxActorType::Enum actorType);
};
}  // namespace Aftr
