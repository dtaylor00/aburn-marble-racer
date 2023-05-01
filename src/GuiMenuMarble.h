#pragma once

#include "AftrConfig.h"
#ifdef AFTR_CONFIG_USE_IMGUI

#include "GuiMenuAbstract.h"

namespace Aftr {
class WorldContainer;
class WOPhysicsMarble;

class GuiMenuMarble : public GuiMenuAbstract {
   public:
    static GuiMenuMarble *New(WorldContainer *worldLst);
    virtual ~GuiMenuMarble();

    virtual void draw() override;
    virtual void render(const Camera &cam) override;
    virtual void update() override;

   protected:
    virtual void onCreate(WorldContainer *worldLst);
    GuiMenuMarble();

   private:
    WorldContainer *worldLst;
    float position[3];
    float offset[6];  // [xmin, xmax, ymin, ymax, zmin, zmax]
    int selectedId;
    WOPhysicsMarble *selectedMarble;
};

}  // namespace Aftr

#endif