#include "AftrConfig.h"
#ifdef AFTR_CONFIG_USE_IMGUI

#include "AftrImGuiIncludes.h"
#include "GuiMenuLight.h"
#include "WOLight.h"

using namespace Aftr;

class Camera;

GuiMenuLight *GuiMenuLight::New(WOLight *light) {
    GuiMenuLight *menu = new GuiMenuLight();
    menu->onCreate(light);
    return menu;
}

GuiMenuLight::~GuiMenuLight() {}
GuiMenuLight::GuiMenuLight() : light(nullptr), colors{0.f, 0.f, 0.f, 0.f} {}

void GuiMenuLight::onCreate(WOLight *light) {
    this->light = light;
    aftrColor4f colors = light->getColor();
    setColors(colors.r, colors.g, colors.b, colors.a);
}

void GuiMenuLight::draw() {
    ImGui::Begin("Light Menu");
    ImGui::ColorEdit4("Color", colors);

    if (ImGui::Button("Day Preset")) setColors(1.f, 1.f, 1.f, 1.f);
    ImGui::SameLine();
    if (ImGui::Button("Evening Preset")) setColors(0.5f, 0.45f, 0.15f, 1.f);
    ImGui::SameLine();
    if (ImGui::Button("Night Preset")) setColors(0.3f, 0.3f, 0.6f, 1.f);

    ImGui::End();
}

void GuiMenuLight::render(const Camera &cam) {}
void GuiMenuLight::update() {
    aftrColor4f colors = aftrColor4f(this->colors[0], this->colors[1], this->colors[2], this->colors[3]);
    this->light->setColor(colors);
}

void GuiMenuLight::setColors(float r, float g, float b, float a) {
    this->colors[0] = r;
    this->colors[1] = g;
    this->colors[2] = b;
    this->colors[3] = a;
    this->light->setColor(aftrColor4f(r, g, b, a));
}

#endif