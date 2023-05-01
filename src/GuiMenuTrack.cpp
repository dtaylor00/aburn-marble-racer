#include "GuiMenuTrack.h"

#ifdef AFTR_CONFIG_USE_IMGUI

#include <iostream>

#include "AftrImGuiIncludes.h"
#include "Camera.h"
#include "GLSLShaderDefaultGL32.h"
#include "GLView.h"
#include "ManagerPhysics.h"
#include "Model.h"
#include "WO.h"
#include "WOAxes.h"
#include "WOPhysicsGoalBox.h"
#include "WOPhysicsRotatingChamber.h"
#include "WOPhysicsTrack.h"
#include "WorldContainer.h"

using namespace Aftr;

static std::string filename(ManagerEnvironmentConfiguration::getSMM() + "/models/cube4x4x4redShinyPlastic_pp.wrl");
GuiMenuTrack::GuiMenuTrack() : rpy(0, 0, 0), pos(0, 0, 0), scale(1, 1, 1), distance(30.f), dcm(), order{ZAxis, YAxis, XAxis}, locked(true), showProjection(true), showAxes(true), tracks(), selected(nullptr), axes(nullptr), camptrptr(nullptr) {}
GuiMenuTrack::~GuiMenuTrack() {}

GuiMenuTrack* GuiMenuTrack::New() {
    GuiMenuTrack* menu = new GuiMenuTrack();
    menu->onCreate();
    return menu;
}

void Aftr::GuiMenuTrack::addTrack(IFaceWOTrack* track) {
    Tex texture = *ManagerTex::loadTexAsync(ManagerEnvironmentConfiguration::getLMM() + "/images/transparent_blue.png");
    WO* wo = track->getWO();  // Idea: A WOProjection class that takes WO pointer and only renders that WO model with a transparent texture

    wo->upon_async_model_loaded([wo, track, texture] {
        wo->getModel()->getSkin().getMultiTextureSet().at(0) = texture;
        track->removeAllActors();
    });

    size_t sz = wo->getChildren().size();
    for (size_t i = 0; i < sz; i++) {
        WO* childwo = wo->getChildren().at(i);
        IFaceWOTrack* childtrack = dynamic_cast<IFaceWOTrack*>(childwo);
        if (childtrack != nullptr) {
            childwo->upon_async_model_loaded([childwo, childtrack, texture] {
                childwo->getModel()->getSkin().getMultiTextureSet().at(0) = texture;
                childtrack->removeAllActors();
            });
        }
    }

    tracks.push_back(track);
}

void GuiMenuTrack::onCreate() {
    this->dcm.setMeToIdentity();
    this->camptrptr = ManagerGLView::getGLView()->getCameraPtrPtr();
    this->axes = WOAxes::New(Vector(10, 10, 10), 0.5f);

    {
        std::string straight_track(ManagerEnvironmentConfiguration::getLMM() + "/models/straight_track.dae");
        std::string curved_track_left(ManagerEnvironmentConfiguration::getLMM() + "/models/curved_track_left.dae");
        std::string curved_track_right(ManagerEnvironmentConfiguration::getLMM() + "/models/curved_track_right.dae");
        std::string funnel_track(ManagerEnvironmentConfiguration::getLMM() + "/models/funnel_track.dae");

        addTrack(WOPhysicsTrack::New("Funnel", funnel_track));
        addTrack(WOPhysicsTrack::New("Straight Track", straight_track));
        addTrack(WOPhysicsTrack::New("Curved Track (Left)", curved_track_left));
        addTrack(WOPhysicsTrack::New("Curved Track (Right)", curved_track_right));
        addTrack(WOPhysicsRotatingChamber::New());
        addTrack(WOPhysicsGoalBox::New());
    }
}

void GuiMenuTrack::draw() {
    const char* trackName = selected == nullptr ? "None" : selected->getTrackName();
    if (ImGui::BeginCombo("Tracks", trackName)) {
        if (ImGui::Selectable("None")) {
            selected = nullptr;
        }

        for (auto track : tracks) {
            if (ImGui::Selectable(track->getTrackName())) {
                selected = track;
            }
        }
        ImGui::EndCombo();
    }

    std::string orderString;
    for (size_t i = 0; i < 3; i++) {
        switch (order[i]) {
            case Axis::XAxis: orderString += "X"; break;
            case Axis::YAxis: orderString += "Y"; break;
            case Axis::ZAxis: orderString += "Z"; break;
            default: break;
        }
    }

    if (ImGui::BeginCombo("Euler Order", orderString.c_str())) {
        if (ImGui::Selectable("ZYX")) setOrder(Axis::ZAxis, Axis::YAxis, Axis::XAxis);
        if (ImGui::Selectable("ZXY")) setOrder(Axis::ZAxis, Axis::XAxis, Axis::YAxis);
        if (ImGui::Selectable("YZX")) setOrder(Axis::YAxis, Axis::ZAxis, Axis::XAxis);
        if (ImGui::Selectable("YXZ")) setOrder(Axis::YAxis, Axis::XAxis, Axis::ZAxis);
        if (ImGui::Selectable("XZY")) setOrder(Axis::XAxis, Axis::ZAxis, Axis::YAxis);
        if (ImGui::Selectable("XYZ")) setOrder(Axis::XAxis, Axis::YAxis, Axis::ZAxis);
        ImGui::EndCombo();
    }

    for (size_t i = 0; i < 3; i++) {
        switch (order[i]) {
            case Axis::XAxis: ImGui::SliderAngle("X-Axis", &rpy.at(0), -180.f, 180.f); break;
            case Axis::YAxis: ImGui::SliderAngle("Y-Axis", &rpy.at(1), -180.f, 180.f); break;
            case Axis::ZAxis: ImGui::SliderAngle("Z-Axis", &rpy.at(2), -180.f, 180.f); break;
            default: break;
        }
    }

    locked ? ImGui::SliderFloat("Distance", &distance, 10, 100) : ImGui::SliderFloat3("Position", &pos.at(0), -100, 100);
    ImGui::SliderFloat3("Scale", &scale.at(0), 1, 5);
    ImGui::Checkbox("Lock to camera", &locked);
    ImGui::Checkbox("Show Projection", &showProjection);
    ImGui::SameLine();
    ImGui::Checkbox("Show Axes", &showAxes);

    ImGui::BeginDisabled(selected == nullptr);
    if (ImGui::Button("Spawn Track")) {
        static int i = 0;
        dcm.setPosition(pos);
        std::string label = std::format("track{:02d}", i);
        WO* wo = selected->clone(dcm, scale);
        wo->setLabel(label);
        ManagerGLView::getGLView()->getWorldContainer()->push_back(wo);
    }
    ImGui::EndDisabled();
}

void GuiMenuTrack::render(const Camera& cam) {
    if (selected != nullptr) {
        if (showProjection) selected->getWO()->render(cam);
        if (showAxes) axes->render(cam);
    }
}

void GuiMenuTrack::update() {
    if (selected != nullptr) {
        dcm = calculateDisplayMatrix();
        selected->getWO()->setDisplayMatrix(dcm);
        axes->setDisplayMatrix(dcm);

        if (locked) pos = (**camptrptr).getPosition() + (distance * (**camptrptr).getLookDirection());
        selected->getWO()->setPosition(pos);
        axes->setPosition(pos);
        selected->getWO()->getModel()->setScale(scale);
    }
}

Mat4 GuiMenuTrack::calculateDisplayMatrix() {
    // https://en.wikipedia.org/wiki/Rotation_matrix#General_rotations
    const float alpha = rpy[2];
    const float beta = rpy[1];
    const float gamma = rpy[0];

    Mat4 yaw = Mat4(Vector(cosf(alpha), sinf(alpha), 0),       //
                    Vector(-1 * sinf(alpha), cosf(alpha), 0),  //
                    Vector(0, 0, 1));                          //

    Mat4 pitch = Mat4(Vector(cosf(beta), 0, -1 * sinf(beta)),  //
                      Vector(0, 1, 0),                         //
                      Vector(sinf(beta), 0, cosf(beta)));      //

    Mat4 roll = Mat4(Vector(1, 0, 0),                            //
                     Vector(0, cosf(gamma), sinf(gamma)),        //
                     Vector(0, -1 * sinf(gamma), cosf(gamma)));  //

    Mat4 result = Mat4();
    for (size_t i = 0; i < 3; i++) {
        switch (order[i]) {
            case Axis::XAxis: result = result * roll; break;
            case Axis::YAxis: result = result * pitch; break;
            case Axis::ZAxis: result = result * yaw; break;
            default: break;
        }
    }
    return result;
}

void GuiMenuTrack::setOrder(Axis first, Axis second, Axis third) {
    order[0] = first;
    order[1] = second;
    order[2] = third;
    convertAngles(first, second, third);
}

void GuiMenuTrack::convertAngles(Axis first, Axis second, Axis third) {
    // https://www.geometrictools.com/Documentation/EulerAngles.pdf
    // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix

    auto id = [](Axis a, Axis b, Axis c) {
        return a + (3 * b) + (9 * c);
    };

    Vector result;  // roll pitch yaw
    int state = id(first, second, third);
    switch (state) {
        case id(Axis::ZAxis, Axis::YAxis, Axis::XAxis): {  // ZYX
            float yaw = atan2(dcm.at(0, 1), dcm.at(0, 0));
            float pitch = asin(-1 * dcm.at(0, 2));
            float roll = atan2(dcm.at(1, 2), dcm.at(2, 2));
            result = Vector(roll, pitch, yaw);
        } break;
        case id(Axis::ZAxis, Axis::XAxis, Axis::YAxis): {  // ZXY
            float roll = asin(dcm.at(1, 2));
            float pitch = atan2(-1 * dcm.at(0, 2), dcm.at(2, 2));
            float yaw = atan2(-1 * dcm.at(1, 0), dcm.at(1, 1));
            result = Vector(roll, pitch, yaw);
        } break;
        case id(Axis::YAxis, Axis::ZAxis, Axis::XAxis): {  // YZX
            float yaw = asin(dcm.at(0, 1));
            float roll = atan2(-1 * dcm.at(2, 1), dcm.at(1, 1));
            float pitch = atan2(-1 * dcm.at(0, 2), dcm.at(0, 0));
            result = Vector(roll, pitch, yaw);
        } break;
        case id(Axis::YAxis, Axis::XAxis, Axis::ZAxis): {  // YXZ
            float roll = asin(-1 * dcm.at(2, 1));
            float yaw = atan2(dcm.at(0, 1), dcm.at(1, 1));
            float pitch = atan2(dcm.at(2, 0), dcm.at(2, 2));
            result = Vector(roll, pitch, yaw);
        } break;
        case id(Axis::XAxis, Axis::ZAxis, Axis::YAxis): {  // XZY
            float yaw = asin(-1 * dcm.at(1, 0));
            float pitch = atan2(dcm.at(2, 0), dcm.at(0, 0));
            float roll = atan2(dcm.at(1, 2), dcm.at(1, 1));
            result = Vector(roll, pitch, yaw);
        } break;
        case id(Axis::XAxis, Axis::YAxis, Axis::ZAxis): {  // XYZ
            float pitch = asin(dcm.at(2, 0));
            float yaw = atan2(-1 * dcm.at(1, 0), dcm.at(0, 0));
            float roll = atan2(-1 * dcm.at(2, 1), dcm.at(2, 2));
            result = Vector(roll, pitch, yaw);
        } break;
        default: {
            result = Vector(0, 0, 0);
        } break;
    }
    rpy = result;
}

#endif