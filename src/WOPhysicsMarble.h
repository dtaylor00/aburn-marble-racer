#pragma once

#include "IFaceWOPhysics.h"
#include "WOPhysics.h"

namespace Aftr {
class WOPhysicsMarble : public WOPhysics {
   public:
    static WOPhysicsMarble *New(Aftr::Vector scale = Vector(1, 1, 1), MESH_SHADING_TYPE shadingType = MESH_SHADING_TYPE::mstSMOOTH);
    virtual ~WOPhysicsMarble();

    int getMarbleId();
    void setMarbleId(int id);

   protected:
    WOPhysicsMarble();
    virtual void onCreate(Aftr::Vector scale, MESH_SHADING_TYPE shadingType);

    int marbleId;
};
}  // namespace Aftr
