#pragma once

#include "IFaceWOPhysics.h"
#include "WOPhysicsAbstract.h"

namespace Aftr {
class WOPhysicsTriangleMesh : public WOPhysicsAbstract {
   public:
    static WOPhysicsTriangleMesh* New(const std::string& modelFileName, Vector scale = Vector(1, 1, 1), MESH_SHADING_TYPE shadingType = MESH_SHADING_TYPE::mstAUTO);
    virtual ~WOPhysicsTriangleMesh();

   protected:
    WOPhysicsTriangleMesh();
    virtual void onCreate(const std::string& modelFileName, Vector scale = Vector(1, 1, 1), MESH_SHADING_TYPE shadingType = MESH_SHADING_TYPE::mstAUTO);
};
}  // namespace Aftr
