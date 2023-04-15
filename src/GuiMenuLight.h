#pragma once

#include "AftrConfig.h"
#ifdef AFTR_CONFIG_USE_IMGUI

#include "GuiMenuAbstract.h"
#include "Mat4.h"

namespace Aftr {

class WOLight;

class GuiMenuLight : public GuiMenuAbstract {
   public:
    static GuiMenuLight *New(WOLight *light);
    virtual ~GuiMenuLight();

    virtual void draw() override;
    virtual void render(const Camera &cam) override;
    virtual void update() override;

    void setColors(float r, float g, float b, float a);

   protected:
    virtual void onCreate(WOLight *light);
    GuiMenuLight();

   private:
    WOLight *light;
    float colors[4];
};

}  // namespace Aftr

#endif