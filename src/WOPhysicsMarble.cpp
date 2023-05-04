#include "WOPhysicsMarble.h"

#include "ManagerMarble.h"
#include "ManagerPhysics.h"
#include "Model.h"
#include "PxPhysicsAPI.h"

using namespace Aftr;
using namespace physx;

static std::string marble(ManagerEnvironmentConfiguration::getLMM() + "/models/marble.dae");

WOPhysicsMarble::WOPhysicsMarble() : IFace(this), WOPhysics(), marbleId(-1) {
    ManagerMarble::registerMarble(this);
}
WOPhysicsMarble::~WOPhysicsMarble() {}

WOPhysicsMarble *WOPhysicsMarble::New(Vector scale, MESH_SHADING_TYPE shadingType) {
    WOPhysicsMarble *wo = new WOPhysicsMarble();
    wo->onCreate(scale, shadingType);
    return wo;
}

void WOPhysicsMarble::onCreate(Vector scale, MESH_SHADING_TYPE shadingType) {
    WO::onCreate(marble, scale, shadingType);

    this->upon_async_model_loaded([this] {
        Tex tex = ManagerMarble::getRandomTexture();
        this->getModel()->getSkin().getMultiTextureSet().at(0) = tex;
    });

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

int Aftr::WOPhysicsMarble::getMarbleId() {
    return this->marbleId;
}

void Aftr::WOPhysicsMarble::setMarbleId(int id) {
    this->marbleId = id;
}
