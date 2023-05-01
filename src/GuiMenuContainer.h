#pragma once

#include "AftrConfig.h"
#ifdef AFTR_CONFIG_USE_IMGUI

#include <vector>

#include "GuiMenuAbstract.h"

namespace Aftr {

class GuiMenuContainer : public GuiMenuAbstract {
   public:
    static GuiMenuContainer *New();
    virtual ~GuiMenuContainer();

    virtual void draw() override;
    virtual void render(const Camera &cam) override;
    virtual void update() override;

    void addMenu(GuiMenuAbstract *menu);

   protected:
    virtual void onCreate();
    GuiMenuContainer();

   private:
    std::vector<GuiMenuAbstract *> menus;
};

}  // namespace Aftr

#endif