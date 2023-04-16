#include "ManagerPhysics.h"

#include <iostream>

#include "IFaceWOPhysics.h"
#include "PxConfig.h"
#include "PxPhysicsAPI.h"
#include "Vector.h"
#include "pvd/PxPvd.h"

using namespace Aftr;

static PxDefaultAllocator defaultAllocatorCallback;
static PxDefaultErrorCallback defaultErrorCallback;
static bool initialized = false;

const int NUM_THREADS = 2;

PxPhysics* ManagerPhysics::px = nullptr;
PxFoundation* ManagerPhysics::pf = nullptr;
PxScene* ManagerPhysics::scene = nullptr;
PxMaterial* ManagerPhysics::mat = nullptr;
PxPvd* ManagerPhysics::pvd = nullptr;

void ManagerPhysics::init(float gravityScalar, Vector gravityNormalizedVector) {
    if (initialized) {
        std::cout << "ERROR: ManagerPhysics has already been initialized...\n";
        std::cout << "Press ENTER to exit...\n";
        std::cin.get();
        std::abort();
    }

    // Foundation
    ManagerPhysics::pf = PxCreateFoundation(PX_PHYSICS_VERSION, defaultAllocatorCallback, defaultErrorCallback);
    if (pf == nullptr) {
        std::cout << "ERROR: PxCreateFoundation failed, aborting...\n";
        std::abort();
    }

    // Visual Debugger
    ManagerPhysics::pvd = PxCreatePvd(*pf);
    PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate("localhost", 5425, 10);
    pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

    // Physics
    ManagerPhysics::px = PxCreatePhysics(PX_PHYSICS_VERSION, *pf, PxTolerancesScale(), true, pvd);
    if (px == nullptr) {
        std::cout << "ERROR: PxCreatePhysics failed, aborting...\n";
        std::abort();
    }
    PxInitExtensions(*px, pvd);

    // Scene Descriptor
    PxSceneDesc sc(px->getTolerancesScale());
    Vector g = gravityScalar * gravityNormalizedVector;
    sc.filterShader = PxDefaultSimulationFilterShader;
    sc.cpuDispatcher = PxDefaultCpuDispatcherCreate(NUM_THREADS);
    sc.flags |= PxSceneFlag::eENABLE_ACTIVE_ACTORS;
    sc.gravity = PxVec3(g.x, g.y, g.z);

    // Scene
    ManagerPhysics::scene = px->createScene(sc);
    PxPvdSceneClient* pvdClient = scene->getScenePvdClient();
    if (pvdClient != nullptr) {
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
        pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
    }

    // Material
    ManagerPhysics::mat = px->createMaterial(0.5f, 0.5f, 1.0f);
    if (mat == nullptr) {
        std::cout << "ERROR: createMaterial failed, aborting...\n";
        std::abort();
    }

    initialized = true;
}

void ManagerPhysics::shutdown() {
    if (px != nullptr) {
        PxCloseExtensions();

        if (scene != nullptr) scene->release();
        scene = nullptr;

        if (px != nullptr) px->release();
        px = nullptr;

        if (pf != nullptr) pf->release();
        pf = nullptr;
    }
}

void ManagerPhysics::simulate(float seconds) { scene->simulate(seconds); }

void ManagerPhysics::syncWOsFromPhysics() {
    PxU32 errorState = 0;
    scene->fetchResults(true, &errorState);
    if (errorState != 0) std::cout << "PhysX Error State: " << errorState << "\n";

    physx::PxU32 numActors = 0;
    physx::PxActor** actors = scene->getActiveActors(numActors);
    for (PxU32 i = 0; i < numActors; ++i) {
        PxActor* actor = actors[i];

        IFaceWOPhysics* iface = static_cast<IFaceWOPhysics*>(actor->userData);
        if (iface != NULL)
            iface->updatePoseFromPhysics();
    }
}
