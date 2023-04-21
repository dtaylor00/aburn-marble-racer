#include "WOPhysicsTriangleMesh.h"

#include "ManagerPhysics.h"
#include "Model.h"
#include "PxPhysicsAPI.h"

using namespace Aftr;
using namespace physx;

WOPhysicsTriangleMesh::WOPhysicsTriangleMesh() : IFace(this), WOPhysicsAbstract() {}
WOPhysicsTriangleMesh::~WOPhysicsTriangleMesh() {}

WOPhysicsTriangleMesh *WOPhysicsTriangleMesh::New(const std::string &modelFileName, Vector scale, MESH_SHADING_TYPE shadingType) {
    WOPhysicsTriangleMesh *wo = new WOPhysicsTriangleMesh();
    wo->onCreate(modelFileName, scale, shadingType);
    return wo;
}

void WOPhysicsTriangleMesh::onCreate(const std::string &modelFileName, Vector scale, MESH_SHADING_TYPE shadingType) {
    WO::onCreate(modelFileName, scale, shadingType);

    this->upon_async_model_loaded([this] {
        PxShape *shape = ManagerPhysics::createShapeFromModel(this->getModel(), true);

        PxPhysics *px = ManagerPhysics::getPhysics();
        PxMaterial *mat = ManagerPhysics::getDefaultMaterial();
        Mat4 pose = model->getPose();
        PxTransform transform(PxMat44(pose.m.data()));

        PxRigidStatic *rigidbody = px->createRigidStatic(transform);
        rigidbody->attachShape(*shape);
        this->onCreatePhysics(rigidbody);
    });
}
