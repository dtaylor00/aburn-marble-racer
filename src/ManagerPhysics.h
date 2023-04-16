#pragma once
#include "AftrConfig.h"
#include "PxPhysicsAPI.h"
#include "Vector.h"
// #include "pvd/PxPvd.h"

using namespace physx;

namespace Aftr {

class ManagerPhysics {
   public:
    static void init(float gravityScalar = Aftr::GRAVITY, Vector gravityNormalizedVector = Vector(0, 0, -1));
    static void shutdown();

    static void simulate(float seconds);
    static void syncWOsFromPhysics();

    static PxPhysics* getPhysics() { return ManagerPhysics::px; }
    static PxFoundation* getFoundation() { return ManagerPhysics::pf; }
    static PxScene* getScene() { return ManagerPhysics::scene; }
    static PxMaterial* getDefaultMaterial() { return ManagerPhysics::mat; }

   protected:
    static PxPhysics* px;
    static PxFoundation* pf;
    static PxScene* scene;
    static PxMaterial* mat;
    static PxPvd* pvd;
};
}  // namespace Aftr