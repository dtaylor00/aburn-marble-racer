#pragma once

#include "AftrConfig.h"
#ifdef AFTR_CONFIG_USE_IMGUI

#include "GuiMenuAbstract.h"
#include "IFaceWOTrack.h"
#include "Mat4.h"
#include "Vector.h"

namespace Aftr {

class WorldContainer;
class WO;

class GuiMenuTrack : public GuiMenuAbstract {
   public:
    enum Axis {
        XAxis,
        YAxis,
        ZAxis,
    };

    static GuiMenuTrack *New();
    virtual ~GuiMenuTrack();

    virtual void draw() override;
    virtual void render(const Camera &cam) override;
    virtual void update() override;

    virtual Mat4 calculateDisplayMatrix();
    virtual void setOrder(Axis first, Axis second, Axis third);
    virtual void convertAngles(Axis first, Axis second, Axis third);
    virtual void addTrack(IFaceWOTrack *track);

   protected:
    virtual void onCreate();
    GuiMenuTrack();

    Vector rpy;      // x=roll y=pitch z=yaw
    Vector pos;      // x y z
    Vector scale;    // x y z
    float distance;  // distance from camera
    Mat4 dcm;        // rotation matrix
    Axis order[3];   // euler order
    bool locked;     // projection locked wrt camera
    bool showProjection;
    bool showAxes;

    std::vector<IFaceWOTrack *> tracks;

   private:
    IFaceWOTrack *selected;  // selected track
    WO *axes;
    Camera **camptrptr;
};

}  // namespace Aftr

#endif