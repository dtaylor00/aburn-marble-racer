#include "WOPhysicsAbstract.h"

#include "ManagerPhysics.h"
#include "Model.h"
#include "PxPhysicsAPI.h"

using namespace Aftr;
using namespace physx;

WOPhysicsAbstract::WOPhysicsAbstract() : IFace(this), IFaceWOPhysics(this), WO() {}
WOPhysicsAbstract::~WOPhysicsAbstract() {}

void WOPhysicsAbstract::onCreatePhysics(PxActor *actor) {
    this->actor = actor;
    this->actor->userData = static_cast<IFaceWOPhysics *>(this);
    ManagerPhysics::getScene()->addActor(*this->actor);
}

void WOPhysicsAbstract::updatePoseFromPhysics() {
    if (actor == nullptr) return;

    PxActorType::Enum type = actor->getType();
    if (type == PxActorType::eRIGID_DYNAMIC || type == PxActorType::eRIGID_STATIC) {
        PxRigidActor *rigidactor = static_cast<PxRigidActor *>(actor);
        PxTransform pose = rigidactor->getGlobalPose();
        Vector pos = Vector(pose.p.x, pose.p.y, pose.p.z);
        this->setPosition(pos);

        Quat quat = Quat(pose.q.w, pose.q.x, pose.q.y, pose.q.z);
        Mat4 dcm = quat.toRotationMatrix();
        this->setDisplayMatrix(dcm);
    }
}

void WOPhysicsAbstract::updatePhysicsFromPose() {
    if (actor == nullptr) return;

    PxActorType::Enum type = actor->getType();
    if (type == PxActorType::eRIGID_DYNAMIC || type == PxActorType::eRIGID_STATIC) {
        PxRigidActor *rigidactor = static_cast<PxRigidActor *>(actor);
        Mat4 pose = model->getPose();
        PxMat44 pxpose(pose.m.data());
        PxTransform transform(pxpose);
        rigidactor->setGlobalPose(transform);
    }
}

void WOPhysicsAbstract::setPosition(const Vector &newXYZ) {
    WO::setPosition(newXYZ);
    updatePhysicsFromPose();
}

void WOPhysicsAbstract::setPosition(float x, float y, float z) {
    WO::setPosition(x, y, z);
    updatePhysicsFromPose();
}

void WOPhysicsAbstract::moveRelative(const Vector &dXdYdZ) {
    WO::moveRelative(dXdYdZ);
    updatePhysicsFromPose();
}

void WOPhysicsAbstract::setDisplayMatrix(const Mat4 &dcm) noexcept {
    WO::setDisplayMatrix(dcm);
    updatePhysicsFromPose();
}

void WOPhysicsAbstract::setPose(const Mat4 &orientation_w_position) noexcept {
    WO::setPose(orientation_w_position);
    updatePhysicsFromPose();
}
