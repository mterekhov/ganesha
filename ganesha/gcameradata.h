#ifndef SPCGANESHAENGINE_GCAMERADATA_H
#define SPCGANESHAENGINE_GCAMERADATA_H

#include "ganeshatypes.h"
#include "gvector.h"
#include "gquaternion.h"

namespace spcGaneshaEngine {

class GCameraData {
public:
    GCameraData();
    ~GCameraData();

    GQuaternion orientation;
    GPoint positionPoint;
    GPoint centerPoint;
    
    const TFloat DefaultMouseSens = 0.001f;
    TFloat mouseSens = DefaultMouseSens;
    
    const TFloat DefaultKeyboardSpeed = 0.1;
    TFloat keyboardSpeed = DefaultKeyboardSpeed;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GCAMERADATA_H
