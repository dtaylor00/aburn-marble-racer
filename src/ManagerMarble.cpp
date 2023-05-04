#include "ManagerMarble.h"

#include <iostream>

#include "GLView.h"
#include "ManagerPhysics.h"
#include "ManagerRandomNumber.h"
#include "ManagerTex.h"
#include "WOPhysicsMarble.h"
#include "WorldContainer.h"
using namespace Aftr;

int ManagerMarble::numMarbles = 0;
std::vector<WOPhysicsMarble*> ManagerMarble::marbles;
std::vector<bool> ManagerMarble::finished_marbles;
std::vector<Tex> ManagerMarble::marble_texures;

void ManagerMarble::init() {
    std::string lmm = ManagerEnvironmentConfiguration::getLMM();
    addTexture(lmm + "/images/marbletex0.jpg");
    addTexture(lmm + "/images/marbletex1.jpg");
    addTexture(lmm + "/images/marbletex2.jpg");
    addTexture(lmm + "/images/marbletex3.jpg");
    addTexture(lmm + "/images/marbletex4.jpg");
    addTexture(lmm + "/images/marbletex5.jpg");
    addTexture(lmm + "/images/marbletex6.jpg");
    addTexture(lmm + "/images/marbletex7.jpg");
    addTexture(lmm + "/images/marbletex8.jpg");
    addTexture(lmm + "/images/marbletex9.jpg");
}

void ManagerMarble::shutdown() {
    marbles.clear();
    finished_marbles.clear();
    marble_texures.clear();
    numMarbles = 0;
}

void Aftr::ManagerMarble::addTexture(Tex tex) {
    marble_texures.push_back(tex);
}

void ManagerMarble::addTexture(std::string filename) {
    auto tex = ManagerTex::loadTexAsync(filename);
    if (tex == std::nullopt) {
        std::cout << "Could not add marble texture: " << filename << std::endl;
        return;
    }
    addTexture(*tex);
}

Tex ManagerMarble::getRandomTexture() {
    int rndIdx = ManagerRandomNumber::getRandomInt(0, marble_texures.size() - 1);
    return marble_texures.at(rndIdx);
}

void ManagerMarble::registerMarble(WOPhysicsMarble* marble) {
    marbles.push_back(marble);
    finished_marbles.push_back(false);
    marble->setMarbleId(numMarbles);
    ++numMarbles;
}

void ManagerMarble::removeMarble(WOPhysicsMarble* marble) {
    return removeMarble(marble->getMarbleId());
}

void ManagerMarble::removeMarble(int marbleId) {
    if (marbleId > marbles.size()) {
        std::cout << "ManagerMarble::removeMarble called with a marbleId outside range. Ignoring...\n";
        return;
    }

    WOPhysicsMarble* wo = marbles[marbleId];
    ManagerGLView::getGLView()->getWorldContainer()->eraseViaWOptr(wo);
    ManagerPhysics::getScene()->removeActor(*wo->getActor());
    marbles[marbleId] = nullptr;
}

bool ManagerMarble::hasFinished(WOPhysicsMarble* marble) {
    return hasFinished(marble->getMarbleId());
}

bool ManagerMarble::hasFinished(int marbleId) {
    if (marbleId > marbles.size()) {
        std::cout << "ManagerMarble::hasFinished called with a marbleId outside range. Ignoring...\n";
        return false;
    }
    return finished_marbles[marbleId];
}

void ManagerMarble::setFinished(WOPhysicsMarble* marble) {
    setFinished(marble->getMarbleId());
}

void ManagerMarble::setFinished(int marbleId) {
    if (marbleId > marbles.size()) {
        std::cout << "ManagerMarble::setFinished called with a marbleId outside range. Ignoring...\n";
        return;
    }

    finished_marbles[marbleId] = true;
}

WOPhysicsMarble* Aftr::ManagerMarble::getMarble(int marbleId) {
    if (marbleId > marbles.size()) {
        std::cout << "ManagerMarble::getMarble called with a marbleId outside range. Ignoring...\n";
        return nullptr;
    }

    return marbles[marbleId];
}