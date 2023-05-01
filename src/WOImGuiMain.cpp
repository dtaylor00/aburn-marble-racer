#include "AftrConfig.h"
#ifdef AFTR_CONFIG_USE_IMGUI

#include "AftrImGuiIncludes.h"
#include "GLView.h"
#include "MGLAxes.h"
#include "WOImGuiMain.h"
#include "WorldContainer.h"

using namespace Aftr;

WOImGuiMain *WOImGuiMain::New(WOGUI *parentWOGUI, float width, float height) {
    WOImGuiMain *imgui = new WOImGuiMain(parentWOGUI);
    imgui->onCreate(width, height);

    return imgui;
}

WOImGuiMain::~WOImGuiMain() {}
WOImGuiMain::WOImGuiMain(WOGUI *parentWOGUI)
    : IFace(this),
      WOImGuiAbstract(parentWOGUI),
      renderCamera(nullptr) {}

void WOImGuiMain::onCreate(float width, float height) {
    WOImGuiAbstract::onCreate(width, height);
    this->renderCamera = ManagerGLView::getGLView()->getCameraPtrPtr();
}

void WOImGuiMain::drawImGui_for_this_frame() {
    ImGui::Begin("Main Menu");
    ImGui::Text("Nothing to see here...");
    ImGui::End();

    for (GuiMenuAbstract *m : menus) {
        m->draw();
        this->fileDialog_update_after_ImGui_end_is_called();
        m->render(**renderCamera);
    }
    ImGui::Begin("Main Menu");
    ImGui::Text("Current FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::End();
}

void WOImGuiMain::onUpdateWO() {
    for (GuiMenuAbstract *m : menus) {
        m->update();
    }
}

void WOImGuiMain::addMenu(GuiMenuAbstract *menu) {
    menus.push_back(menu);
}

#endif