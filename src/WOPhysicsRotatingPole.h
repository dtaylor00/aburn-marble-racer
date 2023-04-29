#pragma once

#include "WOPhysicsTriangleMesh.h"

namespace Aftr {
class WOPhysicsRotatingPole : public WOPhysicsTriangleMesh {  // technically doesn't need to be triangle mesh child
   public:
    static WOPhysicsRotatingPole* New(WO* parent, Vector scale = Vector(1, 1, 1), MESH_SHADING_TYPE shadingType = MESH_SHADING_TYPE::mstAUTO);
    virtual ~WOPhysicsRotatingPole();
    virtual void onUpdateWO() override;

   protected:
    WOPhysicsRotatingPole();
    virtual void onCreate(WO* parent, Vector scale, MESH_SHADING_TYPE shadingType);
};
}  // namespace Aftr
