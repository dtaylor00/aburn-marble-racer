#include "GuiMenuTimer.h"

#ifdef AFTR_CONFIG_USE_IMGUI
#include <iostream>
#include <sstream>

#include "AftrImGuiIncludes.h"
using namespace Aftr;

GuiMenuTimer::GuiMenuTimer() : begin(time_point()), timer(0) {}
GuiMenuTimer::~GuiMenuTimer() {}

GuiMenuTimer* GuiMenuTimer::New(GameState* state) {
    GuiMenuTimer* menu = new GuiMenuTimer();
    menu->onCreate(state);
    return menu;
}

void Aftr::GuiMenuTimer::onCreate(GameState* state) {
    this->state = state;
}

void Aftr::GuiMenuTimer::draw() {
    using namespace std::chrono;

    ImGui::Begin("Timer");
    std::stringstream ss;
    ss << "timer " << duration_cast<milliseconds>(timer);

    ImGui::Text(ss.str().c_str());

    // Start, Pause, Resume
    switch (*state) {
        case STOPPED:
            if (ImGui::Button("Start")) {
                this->begin = steady_clock::now();
                *state = RUNNING;
            }
            break;
        case RUNNING:
            if (ImGui::Button("Pause")) {
                *state = PAUSED;
            }
            break;
        case PAUSED:
            if (ImGui::Button("Resume")) {
                *state = RUNNING;
            }
            break;
        default:
            std::cout << "Bad enum value. Aborting...\n";
            std::abort();
            break;
    }

    // ImGui::Text("Hello world");
    ImGui::End();
}
void Aftr::GuiMenuTimer::render(const Camera& cam) {}

void Aftr::GuiMenuTimer::update() {
    switch (*state) {
        case RUNNING: {
            time_point current = std::chrono::steady_clock::now();
            timer = current - begin;
        } break;
        case STOPPED:
        case PAUSED: {
            // do nothing
        } break;
        default:
            std::cout << "Bad enum value. Aborting...\n";
            std::abort();
            break;
    }
}

#endif