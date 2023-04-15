#pragma once

#include "AftrConfig.h"
#ifdef AFTR_CONFIG_USE_IMGUI

#include "GuiMenuAbstract.h"
#include "Mat4.h"

namespace Aftr {

class MGLSkyBox;

class GuiMenuSkyBox : public GuiMenuAbstract {
   public:
    static GuiMenuSkyBox *New(MGLSkyBox *skybox);
    virtual ~GuiMenuSkyBox();

    virtual void draw() override;
    virtual void render(const Camera &cam) override;
    virtual void update() override;

   protected:
    virtual void onCreate(MGLSkyBox *skybox);
    GuiMenuSkyBox();

   private:
    MGLSkyBox *skybox;
};

}  // namespace Aftr

#endif