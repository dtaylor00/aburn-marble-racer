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
        // auto x = transform.rotate(PxVec3(0, 0, 0.5f));
        // transform.p.x += 0.1f;
        // transform.q.rotate;
        Vector pos = this->getPosition();
        transform.p = PxVec3(pos.x, pos.y, pos.z);
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
    // WO::onCreate(rotating_pole, scale, shadingType);
    // WOPhysicsTriangleMesh::onCreatePhysics(actor);
    this->setParentWorldObject(parent);
    // Physics creation in done by parent to create joint
}
