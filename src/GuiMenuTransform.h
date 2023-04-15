#pragma once

#include "AftrConfig.h"
#ifdef AFTR_CONFIG_USE_IMGUI

#include "GuiMenuAbstract.h"
#include "Mat4.h"

namespace Aftr {

class WorldContainer;

class GuiMenuTransform : public GuiMenuAbstract {
   public:
    static GuiMenuTransform *New(WorldContainer *worldLst);
    virtual ~GuiMenuTransform();

    virtual void draw() override;
    virtual void render(const Camera &cam) override;
    virtual void update() override;

    void applyTransformation();
    virtual void setWO(WO *wo);
    virtual WO *getWO();

   protected:
    virtual void onCreate(WorldContainer *worldLst);
    GuiMenuTransform();

    Mat4 calculateDisplayMatrix();
    Vector calculatePosition();

    void drawList();
    void drawCheckboxes();
    void drawResetButtons();
    void drawSliders();
    void drawTable();
    void drawRangeInput();

   private:
    WO *selectedWO;
    WorldContainer *worldLst;
    WO *axesWO;

    Vector rpy;  // x=roll y=pitch z=yaw
    Vector pos;  // x y z
    Mat4 pose;

    // use relative values
    bool relativeRotation;
    bool relativePosition;

    // convert from relative to global when switching and vice versa
    bool convertRotation;
    bool convertPosition;

    // lock the values from changing outside menu
    bool lockRotation;
    bool lockPosition;

    // indicates a value has changed
    bool changedRotation;
    bool changedPosition;

    // for disabling sliders
    bool disableYaw;
    bool disableYawPitch;

    // for disable DCM table drawing
    bool disableDcmTable;

    // for position slider min/max
    int minPosSlider;
    int maxPosSlider;
};

}  // namespace Aftr

#endif