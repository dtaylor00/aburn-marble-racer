#pragma once

#include "AftrConfig.h"
#ifdef AFTR_CONFIG_USE_IMGUI

#include <chrono>
#include <utility>

#include "GameState.h"
#include "GuiMenuAbstract.h"
#include "PxPhysicsAPI.h"

namespace Aftr {

using time_point = std::chrono::steady_clock::time_point;
using duration = std::chrono::steady_clock::duration;

class GuiMenuSimulation : public GuiMenuAbstract {
   public:
    static GuiMenuSimulation *New(GameState *state);
    virtual ~GuiMenuSimulation();

    virtual void draw() override;
    virtual void render(const Camera &cam) override;
    virtual void update() override;

   protected:
    virtual void onCreate(GameState *state);
    void onTrigger(physx::PxTriggerPair *pairs, physx::PxU32 count);

    GuiMenuSimulation();
    GameState *state;

   private:
    time_point begin;
    duration timer;
    std::vector<std::pair<std::string, duration>> placements;
    float gravityScalar;
    float gravityVector[3];
};

}  // namespace Aftr

#endif