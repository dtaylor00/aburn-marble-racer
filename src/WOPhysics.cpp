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

// void WOPhysics::onCreatePhysics(PxActor *actor) {
//     this->actor = actor;
//     this->actor->userData = static_cast<IFaceWOPhysics *>(this);
//     ManagerPhysics::getScene()->addActor(*this->actor);
// }

// void WOPhysics::updatePoseFromPhysics() {
//     PxActorType::Enum type = actor->getType();

//     if (type == PxActorType::eRIGID_DYNAMIC || type == PxActorType::eRIGID_STATIC) {
//         PxRigidActor *rigidactor = static_cast<PxRigidActor *>(actor);
//         PxTransform pose = rigidactor->getGlobalPose();
//         Vector pos = Vector(pose.p.x, pose.p.y, pose.p.z);
//         this->setPosition(pos);

//         Quat quat = Quat(pose.q.w, pose.q.x, pose.q.y, pose.q.z);
//         Mat4 dcm = quat.toRotationMatrix();
//         this->setDisplayMatrix(dcm);
//     }
// }

// void WOPhysics::setPosition(const Vector &newXYZ) {
//     WO::setPosition(newXYZ);
//     PxRigidActor *body = static_cast<PxRigidActor *>(actor);
//     PxTransform pose = body->getGlobalPose();
//     pose.p = PxVec3(newXYZ.x, newXYZ.y, newXYZ.z);
//     body->setGlobalPose(pose);
// }

// void WOPhysics::setPosition(float x, float y, float z) {
//     this->setPosition(Vector(x, y, z));
// }

// void WOPhysics::setDisplayMatrix(const Mat4 &dcm) noexcept {
//     WO::setDisplayMatrix(dcm);

//     PxRigidActor *body = static_cast<PxRigidActor *>(actor);
//     PxTransform pose = body->getGlobalPose();
//     Quat quat = dcm.toQuat_fromRotationMat();
//     pose.q = PxQuat(quat.b, quat.c, quat.d, quat.a);
//     body->setGlobalPose(pose);
// }

// void WOPhysics::setPose(const Mat4 &orientation_w_position) noexcept {
//     WO::setPose(orientation_w_position);

//     PxRigidActor *body = static_cast<PxRigidActor *>(actor);
//     PxTransform pose = body->getGlobalPose();
//     Vector pos = orientation_w_position.getPosition();
//     Quat quat = orientation_w_position.toQuat_fromRotationMat();
//     pose.p = PxVec3(pos.x, pos.y, pos.z);
//     pose.q = PxQuat(quat.a, quat.b, quat.c, quat.d);
//     body->setGlobalPose(pose);
// }