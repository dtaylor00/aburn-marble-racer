#pragma once

#include "GLView.h"

namespace Aftr {
class Camera;
class WOLight;
class MGLSkyBox;
class WOImGuiMain;

/**
   \class GLViewMarbleRacer
   \author Scott Nykl
   \brief A child of an abstract GLView. This class is the top-most manager of
   the module.

   Read \see GLView for important constructor and init information.

   \see GLView

    \{
*/

class GLViewMarbleRacer : public GLView {
   public:
    static GLViewMarbleRacer *New(const std::vector<std::string> &outArgs);
    virtual ~GLViewMarbleRacer();
    virtual void updateWorld();  ///< Called once per frame
    virtual void loadMap();      ///< Called once at startup to build this module's scene

   protected:
    GLViewMarbleRacer(const std::vector<std::string> &args);
    virtual void onCreate();

   private:
    WOImGuiMain *maingui;
};

/** \} */

}  // namespace Aftr
