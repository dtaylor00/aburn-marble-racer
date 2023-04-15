#pragma once

namespace Aftr {

class Camera;

class GuiMenuAbstract {
   public:
    virtual void render(const Camera& cam) = 0;  // for rendering WO
    virtual void draw() = 0;                     // for drawing ImGui elements
    virtual void update() = 0;                   // for updating state
};
}  // namespace Aftr