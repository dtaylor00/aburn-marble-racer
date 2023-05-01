#include "WOPhysicsTrack.h"

#include "ManagerPhysics.h"
#include "Model.h"
using namespace Aftr;

WOPhysicsTrack::WOPhysicsTrack() : IFace(this), WOPhysicsTriangleMesh() {}

WOPhysicsTrack::~WOPhysicsTrack() {}

WOPhysicsTrack* WOPhysicsTrack::New(const std::string& trackName, const std::string& modelFileName, Vector scale, MESH_SHADING_TYPE shadingType, physx::PxActorType::Enum actorType) {
    WOPhysicsTrack* wo = new WOPhysicsTrack();
    wo->onCreate(trackName, modelFileName, scale, shadingType, actorType);
    return wo;
}

void WOPhysicsTrack::onCreate(const std::string& trackName, const std::string& modelFileName, Vector scale, MESH_SHADING_TYPE shadingType, physx::PxActorType::Enum actorType) {
    WOPhysicsTriangleMesh::onCreate(modelFileName, scale, shadingType, actorType);
    this->setTrackName(trackName.c_str());
}

WO* WOPhysicsTrack::clone() {
    std::string trackName = this->getTrackName();
    std::string modelFileName = model->getFileName();
    Vector scale = model->getScale();

    WO* wo = WOPhysicsTrack::New(trackName, modelFileName, scale);
    return wo;
}

WO* WOPhysicsTrack::clone(Mat4 pose, Vector scale) {
    std::string trackName = this->getTrackName();
    std::string modelFileName = model->getFileName();

    WO* wo = WOPhysicsTrack::New(trackName, modelFileName, scale);
    wo->setPose(pose);
    return wo;
}

void WOPhysicsTrack::removeAllActors() {
    ManagerPhysics::getScene()->removeActor(*this->actor);
}