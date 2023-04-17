#include "AftrConfig.h"
#ifdef AFTR_CONFIG_USE_IMGUI

#include "AftrImGuiIncludes.h"
#include "GLView.h"
#include "GuiMenuTransform.h"
#include "MGLAxes.h"
#include "WOAxes.h"
#include "WorldContainer.h"

using namespace Aftr;

Vector getRpyFromDisplayMatrix(Mat4 dcm);
Vector getGlobalRpyFromDisplayMatrix(Mat4 dcm);

GuiMenuTransform *GuiMenuTransform::New(WorldContainer *worldLst) {
    GuiMenuTransform *menu = new GuiMenuTransform();
    menu->onCreate(worldLst);
    return menu;
}

GuiMenuTransform::~GuiMenuTransform() {}
GuiMenuTransform::GuiMenuTransform()
    : selectedWO(nullptr),
      worldLst(nullptr),
      rpy{0.f, 0.f, 0.f},
      pos{0.f, 0.f, 0.f},
      relativeRotation(true),
      relativePosition(false),
      convertRotation(true),
      convertPosition(true),
      lockRotation(false),
      lockPosition(false),
      disableYaw(false),
      disableYawPitch(false),
      disableDcmTable(false),
      minPosSlider(-100),
      maxPosSlider(100) {}

void GuiMenuTransform::onCreate(WorldContainer *worldLst) {
    this->worldLst = worldLst;
    this->axesWO = WOAxes::New(Vector(10, 10, 10), 0.5f);
}

void GuiMenuTransform::render(const Camera &cam) {
    this->axesWO->render(cam);
}

void GuiMenuTransform::draw() {
    ImGui::Begin("Transform Menu");
    drawList();
    ImGui::BeginDisabled(selectedWO == nullptr);
    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
        drawSliders();
        drawResetButtons();
        if (!disableDcmTable) drawTable();
    }

    if (ImGui::CollapsingHeader("Transform Options")) {
        drawCheckboxes();
        drawRangeInput();
    }
    ImGui::EndDisabled();
    ImGui::End();
}

void GuiMenuTransform::update() {
    this->applyTransformation();
    this->axesWO->onUpdateWO();
}

void GuiMenuTransform::drawList() {
    if (worldLst == nullptr) return;

    const char *selected = selectedWO != nullptr && !selectedWO->getLabel().empty()
                               ? selectedWO->getLabel().c_str()
                               : nullptr;

    if (ImGui::BeginCombo("Selected WO", selected)) {
        for (size_t i = 0; i < this->worldLst->size(); i++) {
            WO *wo = this->worldLst->at(i);
            if (!wo->getLabel().empty() && ImGui::Selectable(wo->getLabel().c_str())) {
                this->setWO(wo);
            }
        }
        ImGui::EndCombo();
    }
}

void GuiMenuTransform::drawSliders() {
    relativeRotation ? ImGui::Text("Relative Rotation") : ImGui::Text("Global Rotation");
    ImGui::BeginDisabled(disableYaw | disableYawPitch);
    changedRotation |= ImGui::SliderAngle("Z-Axis", &rpy.at(2), -180.f, 180.f);
    ImGui::EndDisabled();

    ImGui::BeginDisabled(disableYawPitch);
    // changedRotation |= ImGui::SliderAngle("Y-Axis", &rpy.at(1), -90.f, 90.f);
    changedRotation |= ImGui::SliderAngle("Y-Axis", &rpy.at(1), -180.f, 180.f);
    ImGui::EndDisabled();

    changedRotation |= ImGui::SliderAngle("X-Axis", &rpy.at(0), -180.f, 180.f);

    relativePosition ? ImGui::Text("Relative Position") : ImGui::Text("Global Position");
    float spos[3] = {pos.x, pos.y, pos.z};
    changedPosition |= ImGui::SliderFloat3("Position", spos, minPosSlider, maxPosSlider);
    pos = spos;

    disableYaw = rpy[1] != 0;
    disableYawPitch = rpy[0] != 0;
}

void GuiMenuTransform::drawCheckboxes() {
    if (ImGui::Checkbox("Relative Rotation", &relativeRotation) && convertRotation && lockRotation) {
        // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
        // XYZ vs ZYX
        // convert the current rotation to the alternative rotation schema
        rpy = relativeRotation ? getRpyFromDisplayMatrix(pose)
                               : rpy = getGlobalRpyFromDisplayMatrix(pose);
    }

    ImGui::SameLine();

    if (ImGui::Checkbox("Relative Position", &relativePosition) && convertPosition && lockPosition) {
        // convert the current position to the alternative position schema
        pos = relativePosition ? pose.transposeUpperLeft3x3() *pos
                               : pos = pose * pos;
    }

    changedRotation |= ImGui::Checkbox("Keep Rotation", &convertRotation);
    ImGui::SameLine();
    changedPosition |= ImGui::Checkbox("Keep Position", &convertPosition);

    changedRotation |= ImGui::Checkbox("Lock Rotation", &lockRotation);
    ImGui::SameLine();
    changedPosition |= ImGui::Checkbox("Lock Position", &lockPosition);

    ImGui::Checkbox("Hide DCM Table", &disableDcmTable);
}

void GuiMenuTransform::drawResetButtons() {
    if (ImGui::Button("Reset Roll")) {
        rpy[0] = 0;
        changedRotation = true;
    }

    ImGui::SameLine();
    if (ImGui::Button("Reset Pitch + Roll")) {
        rpy[0] = rpy[1] = 0;
        changedRotation = true;
    }

    // ImGui::SameLine();

    if (ImGui::Button("Reset Rotation")) {
        rpy[0] = rpy[1] = rpy[2] = 0;
        changedRotation = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset Position")) {
        pos[0] = pos[1] = pos[2] = 0;
        changedPosition = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset All")) {
        pos[0] = pos[1] = pos[2] = 0;
        rpy[0] = rpy[1] = rpy[2] = 0;
        changedPosition = changedRotation = true;
    }
}

void GuiMenuTransform::drawTable() {
    // renders the dcm matrix with position
    if (ImGui::BeginTable("Pose", 4)) {
        for (int row = 0; row < 4; row++) {
            ImGui::TableNextRow();
            for (int col = 0; col < 4; col++) {
                ImGui::TableSetColumnIndex(col);
                ImVec4 color;
                switch (col) {
                    case 0:
                        color = ImVec4(1.f, 0.f, 0.f, 1.f);
                        break;
                    case 1:
                        color = ImVec4(0.f, 1.f, 0.f, 1.f);
                        break;
                    case 2:
                        color = ImVec4(0.f, 0.f, 1.f, 1.f);
                        break;
                    default:
                        color = ImVec4(1.f, 1.f, 1.f, 1.f);
                        break;
                }
                if (row == 3) color = ImVec4(1.f, 1.f, 1.f, 1.f);

                ImGui::TextColored(color, "%.2f", pose.at(col, row));
            }
        }
        ImGui::EndTable();
    }
}

void GuiMenuTransform::drawRangeInput() {
    static int range[] = {minPosSlider, maxPosSlider};
    ImGui::InputInt2("Position Range", range);
    minPosSlider = range[0];
    maxPosSlider = range[1];
}

void GuiMenuTransform::applyTransformation() {
    if (selectedWO == nullptr) return;

    // Only set position if a value changed or preventing changes
    if (changedRotation || lockRotation || changedPosition || lockPosition) {
        if (changedRotation || lockRotation) selectedWO->setDisplayMatrix(calculateDisplayMatrix());
        if (changedPosition || lockPosition) selectedWO->setPosition(calculatePosition());
    }

    // Update values to external changes if no changes nor locked
    if (!changedPosition && !lockPosition) {
        pos = relativePosition ? pose.transposeUpperLeft3x3() * selectedWO->getPosition()
                               : selectedWO->getPosition();
    }

    if (!changedRotation && !lockRotation) {
        Mat4 dcm = selectedWO->getDisplayMatrix();
        rpy = relativeRotation ? getRpyFromDisplayMatrix(dcm)
                               : getGlobalRpyFromDisplayMatrix(dcm);
    }
    pose = selectedWO->getPose();
    this->axesWO->setPose(pose);

    // only unset after finish modifying slider
    changedPosition = changedPosition && ImGui::IsMouseDown(ImGuiMouseButton_Left);
    changedRotation = changedRotation && ImGui::IsMouseDown(ImGuiMouseButton_Left);
}

void GuiMenuTransform::setWO(WO *wo) {
    this->selectedWO = wo;
    pos = wo->getPosition();
    rpy = getRpyFromDisplayMatrix(wo->getDisplayMatrix());
}

WO *GuiMenuTransform::getWO() {
    return this->selectedWO;
}

Mat4 GuiMenuTransform::calculateDisplayMatrix() {
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

    Mat4 result = relativeRotation ? yaw * pitch * roll : roll * pitch * yaw;
    return result;
}

Vector GuiMenuTransform::calculatePosition() {
    if (relativePosition) return pose * pos;
    return pos;
}

Vector getRpyFromDisplayMatrix(Mat4 dcm) {
    float roll = atan2(dcm.at(1, 2), dcm.at(2, 2));
    float pitch = asin(-1 * dcm.at(0, 2));
    float yaw = atan2(dcm.at(0, 1), dcm.at(0, 0));

    return Vector(roll, pitch, yaw);
}

Vector getGlobalRpyFromDisplayMatrix(Mat4 dcm) {
    // float yaw = atan2(dcm.at(2, 1), dcm.at(2, 2));
    float yaw = atan2(-1 * dcm.at(2, 1), dcm.at(2, 2));
    float pitch = asin(dcm.at(2, 0));
    float roll = atan2(-1 * dcm.at(1, 0), dcm.at(0, 0));

    return Vector(yaw, pitch, roll);
}

#endif