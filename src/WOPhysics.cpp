#include "WOPhysics.h"

#include "ManagerPhysics.h"
#include "PxPhysicsAPI.h"

using namespace Aftr;
using namespace physx;

WOPhysics::WOPhysics() : IFace(this), WOPhysicsAbstract() {}
WOPhysics::~WOPhysics() {}

WOPhysics *WOPhysics::New(physx::PxActor *actor) {
    WOPhysics *wo = new WOPhysics();
    wo->onCreate(actor);
    return wo;
}

WOPhysics *WOPhysics::New(physx::PxActor *actor, const std::string &modelFileName, Vector scale, MESH_SHADING_TYPE shadingType) {
    WOPhysics *wo = new WOPhysics();
    wo->onCreate(actor, modelFileName, scale, shadingType);
    return wo;
}

void WOPhysics::onCreate(physx::PxActor *actor) {
    WO::onCreate();
    this->onCreatePhysics(actor);
}

void WOPhysics::onCreate(physx::PxActor *actor, const std::string &modelFileName, Vector scale, MESH_SHADING_TYPE shadingType) {
    WO::onCreate(modelFileName, scale, shadingType);
    this->onCreatePhysics(actor);
}
