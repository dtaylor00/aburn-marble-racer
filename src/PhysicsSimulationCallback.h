#pragma once

#include <functional>

#include "PxSimulationEventCallback.h"

using namespace physx;

namespace Aftr {
class PhysicsSimulationCallback : public PxSimulationEventCallback {
   public:
    PhysicsSimulationCallback();
    ~PhysicsSimulationCallback();

    virtual void onTrigger(PxTriggerPair* pairs, PxU32 count) override;
    virtual void onConstraintBreak(PxConstraintInfo* constraints, PxU32 count) override{};
    virtual void onWake(PxActor** actors, PxU32 count) override{};
    virtual void onSleep(PxActor** actors, PxU32 count) override{};
    virtual void onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pairs, PxU32 nbPairs) override{};
    virtual void onAdvance(const PxRigidBody* const* bodyBuffer, const PxTransform* poseBuffer, const PxU32 count) override{};

    using Callback_onTrigger = std::function<void(PxTriggerPair*, PxU32)>;
    void subscribe_onTrigger(Callback_onTrigger callback);

   private:
    std::vector<Callback_onTrigger> subscribers_onTrigger;
};
}  // namespace Aftr