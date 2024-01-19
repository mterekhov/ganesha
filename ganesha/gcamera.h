#ifndef SPCGANESHAENGINE_GCAMERA_H
#define SPCGANESHAENGINE_GCAMERA_H

#include "ganeshatypes.h"
#include "gvector.h"
#include "gmatrix.h"
#include "gquaternion.h"
#include "glog.h"

namespace spcGaneshaEngine {

/// Manipulates with current position
class GCamera {
public:
    GCamera();
    ~GCamera();
    
    GMatrix viewMatrix();

    void mouseCamera(const TFloat diff_x, const TFloat diff_y);
    void strafeRightCamera();
    void strafeLeftCamera();
    void upCamera();
    void downCamera();
    void forwardCamera();
    void backwardCamera();

private:
    GQuaternion orientation;
    GPoint positionPoint;
    GPoint centerPoint;
    
    const TFloat DefaultMouseSens = 0.001f;
    TFloat mouseSens = DefaultMouseSens;
    
    const TFloat DefaultKeyboardSpeed = 0.1;
    TFloat keyboardSpeed = DefaultKeyboardSpeed;

    TFloat aroundX(const TFloat diff, const TFloat currentAngle);
    TFloat aroundY(const TFloat diff, const TFloat currentAngle);
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GCAMERA_H
