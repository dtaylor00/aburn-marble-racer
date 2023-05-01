#include "IFaceWOTrack.h"

#include <cstring>

using namespace Aftr;

IFaceWOTrack::IFaceWOTrack() : trackName(nullptr) {}
IFaceWOTrack::~IFaceWOTrack() {
    if (trackName != nullptr) free(trackName);
}

const char* IFaceWOTrack::getTrackName() {
    return trackName;
}

void IFaceWOTrack::setTrackName(const std::string& name) {
    setTrackName(name.c_str());
}

void Aftr::IFaceWOTrack::setTrackName(const char* name) {
    if (trackName != nullptr) free((void*)trackName);
    this->trackName = strdup(name);
}
