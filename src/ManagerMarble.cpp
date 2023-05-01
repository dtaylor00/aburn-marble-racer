#include "ManagerMarble.h"

#include <iostream>

#include "GLView.h"
#include "ManagerPhysics.h"
#include "WOPhysicsMarble.h"
#include "WorldContainer.h"
using namespace Aftr;

int ManagerMarble::numMarbles = 0;
std::vector<WOPhysicsMarble*> ManagerMarble::marbles;
std::vector<bool> ManagerMarble::finished_marbles;

void ManagerMarble::init() {}

void ManagerMarble::shutdown() {
    marbles.clear();
    finished_marbles.clear();
    numMarbles = 0;
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