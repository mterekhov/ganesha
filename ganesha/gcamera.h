#ifndef SPCGANESHAENGINE_GCAMERA_H
#define SPCGANESHAENGINE_GCAMERA_H

#include "ganeshatypes.h"
#include "gvector.h"
#include "gmatrix.h"

namespace spcGaneshaEngine {

/// Manipulates with current position
class GCamera {
public:
    GCamera();
    GCamera(const GPoint& initialPosition, const GPoint& focusPoint, const GVector& initialUpVector);
    GMatrix viewMatrix() const;

    void mouseCamera(const TFloat diff_x, const TFloat diff_y);
    void strafeRightCamera();
    void strafeLeftCamera();
    void upCamera();
    void downCamera();
    void forwardCamera();
    void backwardCamera();

private:
    GPoint positionPoint;
    GPoint centerPoint;
    
    GVector upVector;   //  yaxis
    GVector sightVector;    //  zaxis
    GVector strafeVector;   //  xaxis

    const TFloat DefaultMouseSens = 1.0f;
    TFloat mouseSens = DefaultMouseSens;
    
    const TFloat DefaultKeyboardSpeed = 0.1;
    TFloat keyboardSpeed = DefaultKeyboardSpeed;

    void init(const GPoint& position, const GPoint& center, const GVector& initialUpVector);
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GCAMERA_H
