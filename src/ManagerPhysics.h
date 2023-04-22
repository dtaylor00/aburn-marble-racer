#pragma once
#include "AftrConfig.h"
#include "PhysicsSimulationCallback.h"
#include "PxPhysicsAPI.h"
#include "Vector.h"

using namespace physx;

namespace Aftr {
class Model;

class ManagerPhysics {
   public:
    static void init(float gravityScalar = Aftr::GRAVITY, Vector gravityNormalizedVector = Vector(0, 0, -1));
    static void shutdown();

    static void simulate(float seconds);
    static void syncWOsFromPhysics();

    static PxShape* createShapeFromModel(const Model* model, bool isExclusive = false);

    static PxPhysics* getPhysics() { return ManagerPhysics::px; }
    static PxFoundation* getFoundation() { return ManagerPhysics::pf; }
    static PxScene* getScene() { return ManagerPhysics::scene; }
    static PxMaterial* getDefaultMaterial() { return ManagerPhysics::mat; }
    static PhysicsSimulationCallback* getSimulationCallback() { return ManagerPhysics::simCallback; }

   protected:
    static PxPhysics* px;
    static PxFoundation* pf;
    static PxMaterial* mat;
    static PxPvd* pvd;

    static PxScene* scene;
    static PhysicsSimulationCallback* simCallback;
};
}  // namespace Aftr