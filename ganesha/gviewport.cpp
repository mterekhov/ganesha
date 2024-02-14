#include "gviewport.h"

namespace spcGaneshaEngine {

GViewport::GViewport() : fov(M_PI_4), far(100), near(0.000001), width(1280), height(720) {
    
}

GViewport::~GViewport() {
    
}

};  //  spcGaneshaEngine
