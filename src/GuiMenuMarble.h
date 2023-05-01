#pragma once

#include "AftrConfig.h"
#ifdef AFTR_CONFIG_USE_IMGUI

#include "GuiMenuAbstract.h"

namespace Aftr {
class WOPhysicsMarble;

class GuiMenuMarble : public GuiMenuAbstract {
   public:
    static GuiMenuMarble *New();
    virtual ~GuiMenuMarble();

    virtual void draw() override;
    virtual void render(const Camera &cam) override;
    virtual void update() override;

   protected:
    virtual void onCreate();
    GuiMenuMarble();

   private:
    float position[3];
    float offset[6];  // [xmin, xmax, ymin, ymax, zmin, zmax]
    int selectedId;
    WOPhysicsMarble *selectedMarble;
};

}  // namespace Aftr

#endif