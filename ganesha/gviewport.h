#ifndef SPCGANESHAENGINE_GVIEWPORT_H
#define SPCGANESHAENGINE_GVIEWPORT_H

#include "ganeshatypes.h"

namespace spcGaneshaEngine {

/// Manipulates with current position
class GViewport {
public:
    GViewport();
    ~GViewport();
    
    TFloat width;
    TFloat height;
    TFloat fov;
    TFloat far;
    TFloat near;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GVIEWPORT_H
