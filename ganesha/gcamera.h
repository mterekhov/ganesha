#ifndef SPCGANESHAENGINE_GCAMERA_H
#define SPCGANESHAENGINE_GCAMERA_H

#include "ganeshatypes.h"
#include "gvector.h"
#include "gmatrix.h"

namespace spcGaneshaEngine {

/// Manipulates with current position
class GCamera {
private:
    GPoint positionPoint;
    GPoint centerPoint;
    
    GVector upVector;   //  yaxis
    GVector sightVector;    //  zaxis
    GVector strafeVector;   //  xaxis

    const TFloat DefaultMouseSens = 100.0f;
    TFloat mouseSens = DefaultMouseSens;
    
    TFloat DefaultKeyboardSpeed = 0.1;
    TFloat keyboardSpeed = DefaultKeyboardSpeed;

    void init(const GPoint& position, const GPoint& center, const GVector& initialUpVector);
public:
    GCamera();
    GCamera(const GPoint& initialPosition, const GPoint& focusPoint, const GVector& initialUpVector);
    GMatrix viewMatrix() const;

    void strafeRightCamera();
    void strafeLeftCamera();
    void upCamera();
    void downCamera();
    void forwardCamera();
    void backwardCamera();
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GCAMERA_H
