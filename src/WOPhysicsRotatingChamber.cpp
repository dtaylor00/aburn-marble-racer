#include "WOPhysicsRotatingChamber.h"

#include "ManagerPhysics.h"
#include "Model.h"
#include "PxPhysicsAPI.h"
#include "WOPhysicsRotatingPole.h"

using namespace Aftr;

const std::string rotating_chamber(ManagerEnvironmentConfiguration::getLMM() + "/models/rotating_chamber.dae");

WOPhysicsRotatingChamber::WOPhysicsRotatingChamber() : IFace(this), WOPhysicsTrack() {}
WOPhysicsRotatingChamber::~WOPhysicsRotatingChamber() {}

WOPhysicsRotatingChamber *WOPhysicsRotatingChamber::New(Vector scale, MESH_SHADING_TYPE shadingType, PxActorType::Enum actorType) {
    WOPhysicsRotatingChamber *wo = new WOPhysicsRotatingChamber();
    wo->onCreate(scale, shadingType, actorType);
    return wo;
}

void WOPhysicsRotatingChamber::onCreate(Vector scale, MESH_SHADING_TYPE shadingType, PxActorType::Enum actorType) {
    WOPhysicsTrack::onCreate("Rotating Chamber", rotating_chamber, scale, shadingType, actorType);
    WO *wo = WOPhysicsRotatingPole::New(this, scale, shadingType);
    children.push_back(wo);
}

void WOPhysicsRotatingChamber::setPose(const Mat4 &orientation_w_position) noexcept {
    WOPhysicsTrack::setPose(orientation_w_position);
    for (size_t i = 0; i < children.size(); i++) {
        children[i]->setPose(orientation_w_position);
    }
}

void WOPhysicsRotatingChamber::setDisplayMatrix(const Mat4 &dcm) noexcept {
    WOPhysicsTrack::setDisplayMatrix(dcm);
    for (size_t i = 0; i < children.size(); i++) {
        children[i]->setDisplayMatrix(dcm);
    }
}

WO *WOPhysicsRotatingChamber::clone() {
    Vector scale = model->getScale();
    WO *wo = WOPhysicsRotatingChamber::New(scale);
    return wo;
}

WO *WOPhysicsRotatingChamber::clone(Mat4 pose, Vector scale) {
    WO *wo = WOPhysicsRotatingChamber::New(scale);
    wo->setPose(pose);
    return wo;
}
