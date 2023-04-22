#include "PhysicsSimulationCallback.h"

#include <iostream>

#include "IFaceWOPhysics.h"
#include "PxPhysicsAPI.h"
#include "WOPhysicsMarble.h"

using namespace physx;
using namespace Aftr;

PhysicsSimulationCallback::PhysicsSimulationCallback() : subscribers_onTrigger() {}

PhysicsSimulationCallback::~PhysicsSimulationCallback() {}

void PhysicsSimulationCallback::onTrigger(PxTriggerPair* pairs, PxU32 count) {
    for (const auto& func : subscribers_onTrigger) {
        func(pairs, count);
    }
}

void PhysicsSimulationCallback::subscribe_onTrigger(Callback_onTrigger callback) {
    this->subscribers_onTrigger.push_back(callback);
}
