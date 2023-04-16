#include "WOPhysicsMarble.h"

#include "ManagerPhysics.h"
#include "PxPhysicsAPI.h"

using namespace Aftr;
using namespace physx;

static std::string marble(ManagerEnvironmentConfiguration::getLMM() + "/models/marble.dae");

WOPhysicsMarble::WOPhysicsMarble() : IFace(this), WOPhysics() {}
WOPhysicsMarble::~WOPhysicsMarble() {}

WOPhysicsMarble *WOPhysicsMarble::New(Vector scale, MESH_SHADING_TYPE shadingType) {
    WOPhysicsMarble *wo = new WOPhysicsMarble();
    wo->onCreate(scale, shadingType);
    return wo;
}

void WOPhysicsMarble::onCreate(Vector scale, MESH_SHADING_TYPE shadingType) {
    WO::onCreate(marble, scale);

    PxPhysics *px = ManagerPhysics::getPhysics();
    PxScene *scene = ManagerPhysics::getScene();
    PxMaterial *mat = ManagerPhysics::getDefaultMaterial();

    PxSphereGeometry geom(1.f);
    PxShape *shape = px->createShape(geom, *mat, true);
    PxTransform t({0, 0, 0});

    PxRigidDynamic *body = px->createRigidDynamic(t);
    body->attachShape(*shape);

    // this->onCreatePhysics(body);
    WOPhysicsAbstract::onCreatePhysics(body);
}
