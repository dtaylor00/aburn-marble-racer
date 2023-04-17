#include "WOPhysicsTriangleMesh.h"

#include "ManagerPhysics.h"
#include "Model.h"
#include "PxPhysicsAPI.h"
#include "cooking/PxCooking.h"

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
        PxPhysics *px = ManagerPhysics::getPhysics();
        PxScene *scene = ManagerPhysics::getScene();
        PxMaterial *mat = ManagerPhysics::getDefaultMaterial();

        PxCookingParams params(px->getTolerancesScale());
        params.midphaseDesc.setToDefault(PxMeshMidPhase::eBVH34);
        params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE;
        params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;
        params.meshPreprocessParams |= PxMeshPreprocessingFlag::eENABLE_INERTIA;
        params.meshWeldTolerance = 1e-7f;

        const Model *model = this->getModel();
        const std::vector<Vector> &verts = model->getCompositeVertexList();
        const std::vector<unsigned int> &indices = model->getCompositeIndexList();
        PxTriangleMeshDesc meshDesc;
        meshDesc.points.count = verts.size();
        meshDesc.points.data = verts.data();
        meshDesc.points.stride = sizeof(Vector);
        meshDesc.triangles.count = indices.size() / 3;
        meshDesc.triangles.data = indices.data();
        meshDesc.triangles.stride = 3 * sizeof(unsigned int);

        // bool res = PxValidateTriangleMesh(params, meshDesc);
        // PX_ASSERT(res);

        PxTriangleMesh *mesh = PxCreateTriangleMesh(params, meshDesc, px->getPhysicsInsertionCallback());
        PxTriangleMeshGeometry geom(mesh, PxMeshScale());

        Mat4 pose = model->getPose();
        PxTransform transform(PxMat44(pose.m.data()));
        PxRigidStatic *rigidbody = px->createRigidStatic(transform);
        PxShape *shape = PxRigidActorExt::createExclusiveShape(*rigidbody, geom, *mat);

        this->onCreatePhysics(rigidbody);
    });
}
