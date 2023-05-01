#include "WOPhysicsGoalBox.h"

#include "ManagerPhysics.h"
#include "Model.h"
#include "PxPhysicsAPI.h"

using namespace Aftr;

const std::string goalbox(ManagerEnvironmentConfiguration::getLMM() + "/models/goalbox.dae");

WOPhysicsGoalBox::WOPhysicsGoalBox() : IFace(this), WOPhysicsTrack() {}
WOPhysicsGoalBox::~WOPhysicsGoalBox() {}

WOPhysicsGoalBox *WOPhysicsGoalBox::New(Vector scale, MESH_SHADING_TYPE shadingType, PxActorType::Enum actorType) {
    WOPhysicsGoalBox *wo = new WOPhysicsGoalBox();
    wo->onCreate(scale, shadingType, actorType);
    return wo;
}

void WOPhysicsGoalBox::onCreate(Vector scale, MESH_SHADING_TYPE shadingType, PxActorType::Enum actorType) {
    WOPhysicsTrack::onCreate("Goal Box", goalbox, scale, shadingType, actorType);
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

    WOPhysicsTrack::onCreatePhysics(actor);
}

WO *WOPhysicsGoalBox::clone() {
    Vector scale = model->getScale();
    WO *wo = WOPhysicsGoalBox::New(scale);
    return wo;
}

WO *WOPhysicsGoalBox::clone(Mat4 pose, Vector scale) {
    WO *wo = WOPhysicsGoalBox::New(scale);
    wo->setPose(pose);
    return wo;
}
