#include "WOPhysicsGoalBox.h"

#include "ManagerPhysics.h"
#include "Model.h"
#include "PxPhysicsAPI.h"

using namespace Aftr;

const std::string goalbox(ManagerEnvironmentConfiguration::getLMM() + "/models/goalbox.dae");

WOPhysicsGoalBox::WOPhysicsGoalBox() : IFace(this), WOPhysicsTriangleMesh() {}
WOPhysicsGoalBox::~WOPhysicsGoalBox() {}

WOPhysicsGoalBox *WOPhysicsGoalBox::New(Vector scale, MESH_SHADING_TYPE shadingType) {
    WOPhysicsGoalBox *wo = new WOPhysicsGoalBox();
    wo->onCreate(scale, shadingType);
    return wo;
}

void WOPhysicsGoalBox::onCreate(Vector scale, MESH_SHADING_TYPE shadingType) {
    WOPhysicsTriangleMesh::onCreate(goalbox, scale, shadingType);
}

void WOPhysicsGoalBox::onCreatePhysics(PxActor *actor) {
    PxActorType::Enum type = actor->getType();  // Appending a trigger box on the model bounding box
    if (type == PxActorType::eRIGID_DYNAMIC || type == PxActorType::eRIGID_STATIC) {
        PxRigidActor *rigidactor = static_cast<PxRigidActor *>(actor);
        const Model *model = this->getModel();
        BoundingBox boundingBox = model->getBoundingBox();
        Vector halfExtends = boundingBox.getlxlylz() / 2.f;
        PxBoxGeometry box(halfExtends.x, halfExtends.y, halfExtends.z);
        PxMaterial *mat = ManagerPhysics::getDefaultMaterial();
        PxShapeFlags flags = PxShapeFlag::eVISUALIZATION | PxShapeFlag::eSCENE_QUERY_SHAPE | PxShapeFlag::eTRIGGER_SHAPE;
        PxRigidActorExt::createExclusiveShape(*rigidactor, box, *mat, flags);
    }

    WOPhysicsTriangleMesh::onCreatePhysics(actor);
}