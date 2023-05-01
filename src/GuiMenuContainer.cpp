#include "GuiMenuContainer.h"

#include "AftrImGuiIncludes.h"
using namespace Aftr;

GuiMenuContainer::GuiMenuContainer() : menus() {}
GuiMenuContainer::~GuiMenuContainer() {}

GuiMenuContainer* GuiMenuContainer::New() {
    GuiMenuContainer* menu = new GuiMenuContainer();
    menu->onCreate();
    return menu;
}

void GuiMenuContainer::onCreate() {}

void GuiMenuContainer::draw() {
    for (auto& menu : menus) {
        menu->draw();
        ImGui::Separator();
    }
}

void GuiMenuContainer::render(const Camera& cam) {
    for (auto& menu : menus) menu->render(cam);
}

void GuiMenuContainer::update() {
    for (auto& menu : menus) menu->update();
}

void GuiMenuContainer::addMenu(GuiMenuAbstract* menu) {
    this->menus.push_back(menu);
}