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
WOImGuiMain::WOImGuiMain(WOGUI *parentWOGUI) : IFace(this), WOImGuiAbstract(parentWOGUI), renderCamera(nullptr) {}

void WOImGuiMain::onCreate(float width, float height) {
    WOImGuiAbstract::onCreate(width, height);
    this->renderCamera = ManagerGLView::getGLView()->getCameraPtrPtr();
}

void WOImGuiMain::drawImGui_for_this_frame() {
    const size_t num_menus = menus.size();

    ImGui::Begin("Main Menu");
    if (ImGui::Button("Show All")) {
        for (size_t i = 0; i < num_menus; i++) {
            *states[i] = true;
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Hide All")) {
        for (size_t i = 0; i < num_menus; i++) {
            *states[i] = false;
        }
    }
    ImGui::End();

    for (size_t i = 0; i < num_menus; i++) {
        GuiMenuAbstract *m = menus[i];
        const char *title = titles[i];
        bool *state = states[i];

        if (*state) {
            ImGui::Begin(title, state);
            m->draw();
            ImGui::End();
            this->fileDialog_update_after_ImGui_end_is_called();
            m->render(**renderCamera);
        }

        ImGui::Begin("Main Menu");
        std::string toggleTitle = std::format("Toggle {}", title);
        if (ImGui::Button(toggleTitle.c_str())) {
            *state = !(*state);
        }
        ImGui::End();
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

void WOImGuiMain::addMenu(GuiMenuAbstract *menu, const char *title) {
    menus.push_back(menu);
    titles.push_back(title);
    states.push_back(new bool(true));
}

#endif