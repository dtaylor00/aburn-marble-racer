#include "WOPhysicsRotatingChamber.h"

#include "ManagerPhysics.h"
#include "Model.h"
#include "PxPhysicsAPI.h"
#include "WOPhysicsRotatingPole.h"

using namespace Aftr;

const std::string rotating_chamber(ManagerEnvironmentConfiguration::getLMM() + "/models/rotating_chamber.dae");

WOPhysicsRotatingChamber::WOPhysicsRotatingChamber() : IFace(this), WOPhysicsTriangleMesh() {}
WOPhysicsRotatingChamber::~WOPhysicsRotatingChamber() {}

WOPhysicsRotatingChamber *WOPhysicsRotatingChamber::New(Vector scale, MESH_SHADING_TYPE shadingType, PxActorType::Enum actorType) {
    WOPhysicsRotatingChamber *wo = new WOPhysicsRotatingChamber();
    wo->onCreate(scale, shadingType, actorType);
    return wo;
}

void WOPhysicsRotatingChamber::onCreate(Vector scale, MESH_SHADING_TYPE shadingType, PxActorType::Enum actorType) {
    WOPhysicsTriangleMesh::onCreate(rotating_chamber, scale, shadingType, actorType);
    WOPhysicsTriangleMesh *wo = WOPhysicsRotatingPole::New(this, scale, shadingType);
    children.push_back(wo);
}

void WOPhysicsRotatingChamber::setPose(const Mat4 &orientation_w_position) noexcept {
    WOPhysicsTriangleMesh::setPose(orientation_w_position);
    for (size_t i = 0; i < children.size(); i++) {
        children[i]->setPose(orientation_w_position);
    }
}

void Aftr::WOPhysicsRotatingChamber::setDisplayMatrix(const Mat4 &dcm) noexcept {
    WOPhysicsTriangleMesh::setDisplayMatrix(dcm);
    for (size_t i = 0; i < children.size(); i++) {
        children[i]->setDisplayMatrix(dcm);
    }
}
