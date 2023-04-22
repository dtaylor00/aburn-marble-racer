#include "WOPhysicsMarble.h"

#include "ManagerPhysics.h"
#include "PxPhysicsAPI.h"

using namespace Aftr;
using namespace physx;

static std::string marble(ManagerEnvironmentConfiguration::getLMM() + "/models/marble.dae");

WOPhysicsMarble::WOPhysicsMarble() : IFace(this), WOPhysics(), finished(false) {}
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
    // PxMaterial *mat = ManagerPhysics::getDefaultMaterial();
    PxMaterial *mat = px->createMaterial(0.5f, 0.5f, 0.05f);

    PxSphereGeometry geom(1.f);
    PxShape *shape = px->createShape(geom, *mat, true);
    PxRigidDynamic *body = px->createRigidDynamic(PxTransform(0, 0, 0));
    body->setAngularDamping(0);
    body->setLinearDamping(0);
    body->attachShape(*shape);

    this->onCreatePhysics(body);
}
