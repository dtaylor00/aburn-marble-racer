#include "AftrConfig.h"
#ifdef AFTR_CONFIG_USE_IMGUI

#include "AftrImGuiIncludes.h"
#include "GuiMenuSkyBox.h"
#include "MGLSkyBox.h"

using namespace Aftr;

class Camera;

GuiMenuSkyBox *GuiMenuSkyBox::New(MGLSkyBox *skybox) {
    GuiMenuSkyBox *menu = new GuiMenuSkyBox();
    menu->onCreate(skybox);
    return menu;
}

GuiMenuSkyBox::~GuiMenuSkyBox() {}
GuiMenuSkyBox::GuiMenuSkyBox() : skybox(nullptr) {}

void GuiMenuSkyBox::onCreate(MGLSkyBox *skybox) {
    this->skybox = skybox;
}

void GuiMenuSkyBox::draw() {
    // ImGui::Begin("SkyBox Menu");
    size_t skinsSize = (&skybox->getSkins())->size();
    std::string currentSkinIndex = std::to_string(skybox->getSkinIndex());
    if (ImGui::BeginCombo("Selected", currentSkinIndex.c_str())) {
        for (size_t i = 0; i < skinsSize; i++) {
            std::string skinIndex = std::to_string(i);
            if (ImGui::Selectable(skinIndex.c_str())) {
                skybox->useSkinAtIndex(i);
            }
        }
        ImGui::EndCombo();
    }

    if (ImGui::Button("Previous")) skybox->usePreviousSkin();
    ImGui::SameLine();
    if (ImGui::Button("Next")) skybox->useNextSkin();

    // ImGui::End();
}

void GuiMenuSkyBox::render(const Camera &cam) {}
void GuiMenuSkyBox::update() {}

#endif