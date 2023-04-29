#include "WOPhysicsTriangleMesh.h"

#include "ManagerPhysics.h"
#include "Model.h"
#include "PxPhysicsAPI.h"

using namespace Aftr;
using namespace physx;

WOPhysicsTriangleMesh::WOPhysicsTriangleMesh() : IFace(this), WOPhysicsAbstract() {}
WOPhysicsTriangleMesh::~WOPhysicsTriangleMesh() {}

WOPhysicsTriangleMesh *WOPhysicsTriangleMesh::New(const std::string &modelFileName, Vector scale, MESH_SHADING_TYPE shadingType, PxActorType::Enum actorType) {
    WOPhysicsTriangleMesh *wo = new WOPhysicsTriangleMesh();
    wo->onCreate(modelFileName, scale, shadingType, actorType);
    return wo;
}

void WOPhysicsTriangleMesh::onCreate(const std::string &modelFileName, Vector scale, MESH_SHADING_TYPE shadingType, PxActorType::Enum actorType) {
    WO::onCreate(modelFileName, scale, shadingType);

    this->upon_async_model_loaded([this, actorType] {
        PxShape *shape = ManagerPhysics::createShapeFromModel(this->getModel(), true);

        PxPhysics *px = ManagerPhysics::getPhysics();
        PxMaterial *mat = ManagerPhysics::getDefaultMaterial();
        Mat4 pose = model->getPose();
        PxTransform transform(PxMat44(pose.m.data()));

        PxRigidActor *rigidactor = px->createRigidStatic(transform);
        if (actorType == PxActorType::eRIGID_STATIC)
            rigidactor = px->createRigidStatic(transform);
        else if (actorType == PxActorType::eRIGID_DYNAMIC) {
            rigidactor = px->createRigidDynamic(transform);
            static_cast<PxRigidDynamic *>(rigidactor)->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
        } else {
            std::cout << "Invalid actorType in TriangleMesh creation!\n";
            std::abort();
        }
        rigidactor->attachShape(*shape);
        this->onCreatePhysics(rigidactor);
    });
}
