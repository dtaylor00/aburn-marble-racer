#include "WOPhysicsRotatingPole.h"

#include "ManagerPhysics.h"
#include "Model.h"
#include "PxPhysicsAPI.h"

using namespace Aftr;
using namespace physx;

const std::string rotating_pole(ManagerEnvironmentConfiguration::getLMM() + "/models/rotating_pole.dae");

WOPhysicsRotatingPole::WOPhysicsRotatingPole() : IFace(this), WOPhysicsTriangleMesh() {}
WOPhysicsRotatingPole::~WOPhysicsRotatingPole() {}

float theta = 0;
void Aftr::WOPhysicsRotatingPole::onUpdateWO() {
    if (actor != nullptr) {
        PxRigidDynamic *rigiddynamic = static_cast<PxRigidDynamic *>(actor);
        PxTransform transform = rigiddynamic->getGlobalPose();
        transform.q *= PxQuat(2 * DEGtoRAD, PxVec3(0, 0, 1));
        rigiddynamic->setKinematicTarget(transform);
    }
}

WOPhysicsRotatingPole *WOPhysicsRotatingPole::New(WO *parent, Vector scale, MESH_SHADING_TYPE shadingType) {
    WOPhysicsRotatingPole *wo = new WOPhysicsRotatingPole();
    wo->onCreate(parent, scale, shadingType);
    return wo;
}

void WOPhysicsRotatingPole::onCreate(WO *parent, Vector scale, MESH_SHADING_TYPE shadingType) {
    WOPhysicsTriangleMesh::onCreate(rotating_pole, scale, shadingType, PxActorType::eRIGID_DYNAMIC);
    this->setParentWorldObject(parent);
}
