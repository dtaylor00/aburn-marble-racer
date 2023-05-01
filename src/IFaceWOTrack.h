#pragma once
#include <string>

#include "IFace.h"
#include "Mat4.h"

namespace Aftr {
class WO;

class IFaceWOTrack : public virtual IFace {
   public:
    IFaceWOTrack();
    virtual ~IFaceWOTrack();

    const char* getTrackName();
    void setTrackName(const std::string& name);
    void setTrackName(const char* name);

    virtual WO* clone() = 0;
    virtual WO* clone(Mat4 pose, Vector scale) = 0;
    virtual void removeAllActors() = 0;

   protected:
    char* trackName;
};
}  // namespace Aftr