#include "IFaceWOPhysics.h"

#include "PxActor.h"

using namespace Aftr;

IFaceWOPhysics::IFaceWOPhysics(WO* wo) : IFace(wo), actor(nullptr) {}
IFaceWOPhysics::~IFaceWOPhysics() { this->actor = nullptr; }

physx::PxActor* IFaceWOPhysics::getActor() { return actor; }
void IFaceWOPhysics::setActor(physx::PxActor* actor) { this->actor = actor; }