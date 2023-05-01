#include "GuiMenuMarble.h"

#ifdef AFTR_CONFIG_USE_IMGUI
#include <random>

#include "AftrImGuiIncludes.h"
#include "GLView.h"
#include "ManagerGLView.h"
#include "ManagerMarble.h"
#include "WOPhysicsMarble.h"
#include "WorldContainer.h"

using namespace Aftr;

GuiMenuMarble::GuiMenuMarble() : worldLst(nullptr), position{0, 0, 105}, offset{-8, 8, -8, 8, 0, 0}, selectedId(-1) {}
GuiMenuMarble::~GuiMenuMarble() {}

GuiMenuMarble* GuiMenuMarble::New(WorldContainer* worldLst) {
    GuiMenuMarble* menu = new GuiMenuMarble();
    menu->onCreate(worldLst);
    return menu;
}

void GuiMenuMarble::onCreate(WorldContainer* worldLst) {
    this->worldLst = worldLst;
}

void GuiMenuMarble::draw() {
    if (ImGui::BeginCombo("Selected", selectedId == -1 ? "none" : selectedMarble->getLabel().c_str())) {
        for (size_t i = 0; i < ManagerMarble::getMarbleCount(); i++) {
            WOPhysicsMarble* wo = ManagerMarble::getMarble(i);
            if (wo != nullptr && ImGui::Selectable(wo->getLabel().c_str())) {
                selectedId = wo->getMarbleId();
                selectedMarble = wo;
            }
        }
        ImGui::EndCombo();
    }
    ImGui::BeginDisabled(selectedId == -1);
    if (ImGui::Button("Remove Marble")) {
        ManagerMarble::removeMarble(selectedId);
        selectedId = -1;
    }
    if (ImGui::Button("Teleport Camera")) {
        ManagerGLView::getGLView()->getCamera()->attachCameraToWO(selectedMarble, selectedMarble->getPosition());
    }
    ImGui::EndDisabled();

    ImGui::Separator();
    if (ImGui::Button("Spawn Marble")) {
        std::string label = std::format("marble{:02d}", ManagerMarble::getMarbleCount());
        float x = position[0] + ManagerRandomNumber::getRandomFloat(offset[0], offset[1]);
        float y = position[1] + ManagerRandomNumber::getRandomFloat(offset[2], offset[3]);
        float z = position[2] + ManagerRandomNumber::getRandomFloat(offset[4], offset[5]);

        WO* wo = WOPhysicsMarble::New();
        wo->setLabel(label);
        wo->setPosition(x, y, z);

        this->worldLst->push_back(wo);
    }

    ImGui::InputFloat3("Position", position);
    if (ImGui::CollapsingHeader("Random Position Offset Ranges")) {
        ImGui::InputFloat2("X Range", offset + 0);
        ImGui::InputFloat2("Y Range", offset + 2);
        ImGui::InputFloat2("Z Range", offset + 4);
    }
}

void GuiMenuMarble::render(const Camera& cam) {
    // if (selectedMarble != nullptr) {
    //    // Some kind of glow or highlight. Maybe just axes like the transform menu
    // }
}

void GuiMenuMarble::update() {}

#endif