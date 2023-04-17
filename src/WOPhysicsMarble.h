#pragma once

#include "IFaceWOPhysics.h"
#include "WOPhysics.h"

namespace Aftr {
class WOPhysicsMarble : public WOPhysics {
   public:
    static WOPhysicsMarble *New(Aftr::Vector scale = Vector(1, 1, 1), MESH_SHADING_TYPE shadingType = MESH_SHADING_TYPE::mstAUTO);
    virtual ~WOPhysicsMarble();

   protected:
    WOPhysicsMarble();
    virtual void onCreate(Aftr::Vector scale = Vector(1, 1, 1), MESH_SHADING_TYPE shadingType = MESH_SHADING_TYPE::mstAUTO);
};
}  // namespace Aftr
