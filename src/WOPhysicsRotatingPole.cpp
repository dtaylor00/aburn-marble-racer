#include "WOPhysicsRotatingPole.h"

#include "ManagerPhysics.h"
#include "Model.h"
#include "PxPhysicsAPI.h"

using namespace Aftr;
using namespace physx;

const std::string rotating_pole(ManagerEnvironmentConfiguration::getLMM() + "/models/rotating_pole.dae");

WOPhysicsRotatingPole::WOPhysicsRotatingPole() : IFace(this), WOPhysicsTrack() {}
WOPhysicsRotatingPole::~WOPhysicsRotatingPole() {}

WOPhysicsRotatingPole *WOPhysicsRotatingPole::New(WO *parent, Vector scale, MESH_SHADING_TYPE shadingType) {
    WOPhysicsRotatingPole *wo = new WOPhysicsRotatingPole();
    wo->onCreate(parent, scale, shadingType);
    return wo;
}

void WOPhysicsRotatingPole::onCreate(WO *parent, Vector scale, MESH_SHADING_TYPE shadingType) {
    WOPhysicsTrack::onCreate("Rotating Pole", rotating_pole, scale, shadingType, PxActorType::eRIGID_DYNAMIC);
    this->setParentWorldObject(parent);
}

void Aftr::WOPhysicsRotatingPole::onUpdateWO() {
    if (actor != nullptr) {
        PxRigidDynamic *rigiddynamic = static_cast<PxRigidDynamic *>(actor);
        PxTransform transform = rigiddynamic->getGlobalPose();
        transform.q *= PxQuat(2 * DEGtoRAD, PxVec3(0, 0, 1));
        rigiddynamic->setKinematicTarget(transform);
    }
}