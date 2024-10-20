#ifndef SPCGANESHAENGINE_GCAMERADATA_H
#define SPCGANESHAENGINE_GCAMERADATA_H

#include "ganeshatypes.h"
#include "gvector.h"
#include "gquaternion.h"

namespace spcGaneshaEngine {

enum EMovementDirection {
    MOVEMENT_DIRECTION_UP,
    MOVEMENT_DIRECTION_DOWN,
    MOVEMENT_DIRECTION_FORWARD,
    MOVEMENT_DIRECTION_BACKWARD,
    MOVEMENT_DIRECTION_STRAFE_LEFT,
    MOVEMENT_DIRECTION_STRAFE_RIGHT
};

class GCameraData {
public:
    GCameraData();
    ~GCameraData();

    //  position of camera
    GPoint eyePoint;
    
    //  the point where we are looking to
    GPoint targetPoint;
    
    //  Vertical vector
    GVector upVector;
    
    const TFloat DefaultMouseSens = 0.01f;
    TFloat mouseSens = DefaultMouseSens;
    
    const TFloat DefaultKeyboardSpeed = 0.1;
    TFloat keyboardSpeed = DefaultKeyboardSpeed;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GCAMERADATA_H
