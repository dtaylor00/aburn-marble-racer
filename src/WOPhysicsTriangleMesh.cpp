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
        // std::vector<Vector> vertLst = model->getCompositeVertexList();
        // PxU32 nbVerts = vertLst.size();
        // PxVec3 *verts = new PxVec3[nbVerts];
        // for (size_t i = 0; i < nbVerts; i++) {
        //     Vector v = vertLst.at(i);
        //     verts[i] = PxVec3(v.x, v.y, v.z);
        // }

        // std::vector<unsigned int> idxLst = model->getCompositeIndexList();
        // PxU32 nbIndices = idxLst.size();
        // PxU32 *indices32 = new PxU32[nbIndices];
        // for (size_t i = 0; i < nbIndices; i++) {
        //     unsigned int idx = idxLst.at(i);
        //     indices32[i] = PxU32(idx);
        // }

        // PxTriangleMeshDesc meshDesc;
        // meshDesc.points.count = nbVerts;
        // meshDesc.points.data = verts;
        // meshDesc.points.stride = sizeof(PxVec3);
        // meshDesc.triangles.count = nbIndices / 3;
        // meshDesc.triangles.data = indices32;
        // meshDesc.triangles.stride = 3 * sizeof(PxU32);

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
