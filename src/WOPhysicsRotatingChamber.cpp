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

    // wo->upon_async_model_loaded([this, wo] {
    //     const Model *model = wo->getModel();
    //     PxShape *shape = ManagerPhysics::createShapeFromModel(model, true);

    //     PxPhysics *px = ManagerPhysics::getPhysics();
    //     PxMaterial *mat = ManagerPhysics::getDefaultMaterial();
    //     Mat4 pose = model->getPose();
    //     PxTransform transform(PxMat44(pose.m.data()));

    //     PxRigidDynamic *rigidbody = px->createRigidDynamic(transform);
    //     rigidbody->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
    //     rigidbody->attachShape(*shape);
    //     wo->onCreatePhysics(rigidbody);

    //     // this->createJoint();
    // });

    children.push_back(wo);
}

void WOPhysicsRotatingChamber::onCreatePhysics(PxActor *actor) {
    WOPhysicsTriangleMesh::onCreatePhysics(actor);
    // this->createJoint();
}

void Aftr::WOPhysicsRotatingChamber::createJoint() {
    return;
    if (this->jointCreated) return;

    WOPhysicsAbstract *wo = dynamic_cast<WOPhysicsAbstract *>(children[0]);
    if (wo == nullptr || wo->getModel()->getModelDataShared() == nullptr) return;

    PxActor *childactor = wo->getActor();
    PxActorType::Enum type = actor->getType();
    PxActorType::Enum childtype = childactor->getType();
    if (childtype != PxActorType::eRIGID_DYNAMIC || (type != PxActorType::eRIGID_DYNAMIC && type != PxActorType::eRIGID_STATIC)) return;

    PxRigidActor *rigidactor = static_cast<PxRigidActor *>(actor);
    PxRigidDynamic *rigiddynamic = static_cast<PxRigidDynamic *>(childactor);

    PxVec3 offset = PxVec3(0, 0, 1);
    PxPhysics *px = ManagerPhysics::getPhysics();
    // creating Revolute joint between actors
    PxRevoluteJoint *joint = PxRevoluteJointCreate(*px, rigidactor, PxTransform(offset), rigiddynamic, PxTransform(-offset));
    // joint->set
    joint->setDriveVelocity(0.5f);
    joint->setRevoluteJointFlag(PxRevoluteJointFlag::eDRIVE_ENABLED, true);
    // joint->setRevoluteJointFlag(PxRevoluteJointFlag::eLIMIT_ENABLED, false);

    // rigiddynamic->wakeUp();

    // PxRigidDynamic *actor0 = px->createRigidDynamic(PxTransform(PxVec3(0, 0, 10)));
    // actor0->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
    // PxBoxGeometry box0(1.f, 1.f, 1.f);
    // PxMaterial *mat = ManagerPhysics::getDefaultMaterial();
    // PxRigidActorExt::createExclusiveShape(*actor0, box0, *mat);
    // ManagerPhysics::getScene()->addActor(*actor0);

    // offset = PxVec3(0, 0, 10);
    // PxRevoluteJoint *hinge0 = PxRevoluteJointCreate(*px, NULL, PxTransform(offset), actor0, PxTransform(-offset));

    // hinge0->setDriveVelocity(0.05f);
    // hinge0->setRevoluteJointFlag(PxRevoluteJointFlag::eDRIVE_ENABLED, true);

    this->jointCreated = true;
}
