#include "GuiMenuSimulation.h"

#ifdef AFTR_CONFIG_USE_IMGUI

#include "AftrImGuiIncludes.h"
#include "IFaceWOPhysics.h"
#include "ManagerPhysics.h"
#include "WOPhysicsMarble.h"

using namespace Aftr;

GuiMenuSimulation::GuiMenuSimulation() : begin(time_point()), timer(0), placements(), gravityScalar(0), gravityVector() {}
GuiMenuSimulation::~GuiMenuSimulation() {}

GuiMenuSimulation* GuiMenuSimulation::New(GameState* state) {
    GuiMenuSimulation* menu = new GuiMenuSimulation();
    menu->onCreate(state);
    return menu;
}

void GuiMenuSimulation::onCreate(GameState* state) {
    this->state = state;
    ManagerPhysics::getSimulationCallback()->subscribe_onTrigger([this](PxTriggerPair* pairs, PxU32 count) {
        this->onTrigger(pairs, count);
    });

    PxScene* scene = ManagerPhysics::getScene();
    PxVec3 gravity = scene->getGravity();
    PxVec3 gravityNormalized = gravity.getNormalized();

    if (gravityNormalized.isZero()) gravityScalar = 0;
    else if (gravityNormalized.x != 0) gravityScalar = gravity.x / gravityNormalized.x;
    else if (gravityNormalized.y != 0) gravityScalar = gravity.y / gravityNormalized.y;
    else if (gravityNormalized.z != 0) gravityScalar = gravity.z / gravityNormalized.z;

    gravityVector[0] = gravityNormalized.x;
    gravityVector[1] = gravityNormalized.y;
    gravityVector[2] = gravityNormalized.z;
}

void GuiMenuSimulation::onTrigger(PxTriggerPair* pairs, PxU32 count) {
    std::cout << "ontrigger from menu\n";
    for (PxU32 i = 0; i < count; i++) {
        const PxTriggerPair& pair = pairs[i];

        const PxActor* actor = pair.otherActor;
        IFaceWOPhysics* iface = static_cast<IFaceWOPhysics*>(actor->userData);
        WOPhysicsMarble* marble = dynamic_cast<WOPhysicsMarble*>(iface->getWO());
        if (marble != nullptr && !marble->isFinished()) {
            marble->setFinished(true);
            std::pair pair = std::make_pair(marble->getLabel(), timer);
            placements.push_back(pair);
        }
    }
}

void GuiMenuSimulation::draw() {
    using namespace std::chrono;
    ImGui::Text("time %dms", duration_cast<milliseconds>(timer));

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
    ImGui::SliderFloat("Gravity Scalar", &gravityScalar, -25.f, 25.f);
    ImGui::SliderFloat3("Gravity Vector", gravityVector, -1, 1);

    ImGui::Separator();
    ImGui::Text("Marble Placement");
    if (ImGui::BeginTable("Table", 2)) {
        for (int row = 0; row < placements.size(); row++) {
            auto [label, finishtime] = placements[row];

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("#%.2d %s", row + 1, label.c_str());

            ImGui::TableNextColumn();
            milliseconds finishtime_ms = duration_cast<milliseconds>(finishtime);
            ImGui::Text("%dms", finishtime_ms);
            // auto c = time.count();
            // ImGui::Text("%d::%d::%d", (c % 1'000'000'000) / 1'000'000, (c % 1'000'000) / 1'000, c % 1'000);
        }
        ImGui::EndTable();
    }
}
void GuiMenuSimulation::render(const Camera& cam) {}

void GuiMenuSimulation::update() {
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