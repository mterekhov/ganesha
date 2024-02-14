#ifndef SPCGANESHAENGINE_GCAMERA_H
#define SPCGANESHAENGINE_GCAMERA_H

#include "ganeshatypes.h"
#include "gmatrix.h"
#include "gcameradata.h"

namespace spcGaneshaEngine {

/// Manipulates with current position
class GCamera {
public:
    GCamera();
    ~GCamera();
    
    GMatrix viewMatrix(const GQuaternion& orientation, const GPoint& positionPoint);

    GQuaternion mouseCamera(const GQuaternion& orientation, const TFloat diff_x, const TFloat diff_y, const TFloat mouseSens);

    GPoint upCamera(const GPoint& positionPoint, const TFloat keyboardSpeed);
    GPoint downCamera(const GPoint& positionPoint, const TFloat keyboardSpeed);
    GPoint strafeLeftCamera(const GPoint& positionPoint, const TFloat keyboardSpeed);
    GPoint strafeRightCamera(const GPoint& positionPoint, const TFloat keyboardSpeed);
    GPoint forwardCamera(const GPoint& positionPoint, const TFloat keyboardSpeed);
    GPoint backwardCamera(const GPoint& positionPoint, const TFloat keyboardSpeed);

private:
    TFloat aroundX(const TFloat mouseSens, const TFloat diff, const TFloat currentAngle);
    TFloat aroundY(const TFloat mouseSens, const TFloat diff, const TFloat currentAngle);
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GCAMERA_H
