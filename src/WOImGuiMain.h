#pragma once

#include "AftrConfig.h"
#ifdef AFTR_CONFIG_USE_IMGUI

#include "GuiMenuAbstract.h"
#include "WOImGuiAbstract.h"

namespace Aftr {

class WorldContainer;

class WOImGuiMain : public WOImGuiAbstract {
   public:
    static WOImGuiMain *New(WOGUI *parentWOGUI, float width = 1.0f, float height = 1.0f);
    virtual ~WOImGuiMain();

    virtual void drawImGui_for_this_frame() override;
    virtual void onUpdateWO() override;

    void addMenu(GuiMenuAbstract *menu, const char *title);

   protected:
    virtual void onCreate(float width, float height);
    WOImGuiMain(WOGUI *parentWOGUI);

   private:
    std::vector<GuiMenuAbstract *> menus;
    std::vector<const char *> titles;
    std::vector<bool *> states;

    Camera **renderCamera;
};

}  // namespace Aftr

#endif