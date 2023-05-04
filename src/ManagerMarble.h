#pragma once
#include "Vector.h"

namespace Aftr {
class WOPhysicsMarble;
class Tex;

class ManagerMarble {
   public:
    friend class WOPhysicsMarble;

    static void init();
    static void shutdown();

    static bool hasFinished(WOPhysicsMarble* marble);
    static bool hasFinished(int marbleId);
    static void setFinished(WOPhysicsMarble* marble);
    static void setFinished(int marbleId);
    static void removeMarble(WOPhysicsMarble* marble);
    static void removeMarble(int marbleId);
    static void addTexture(Tex tex);
    static void addTexture(std::string filename);
    static Tex getRandomTexture();
    static WOPhysicsMarble* getMarble(int marbleId);

    static int getMarbleCount() { return ManagerMarble::numMarbles; }

   protected:
    static void registerMarble(WOPhysicsMarble* marble);
    static int numMarbles;
    static std::vector<WOPhysicsMarble*> marbles;
    static std::vector<bool> finished_marbles;
    static std::vector<Tex> marble_texures;
};

}  // namespace Aftr
