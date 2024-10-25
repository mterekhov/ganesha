#ifndef SPCGANESHAENGINE_GCAMERA_H
#define SPCGANESHAENGINE_GCAMERA_H

#include "ganeshatypes.h"
#include "gcameradata.h"
#include "gmatrix.h"

namespace spcGaneshaEngine {

/// Manipulates with current position
class GCamera {
public:
    GCamera(const GCameraData& cameraData);
    ~GCamera();
    
    GMatrix viewMatrix();

    void mouseCamera(const TFloat diff_x, const TFloat diff_y);

    void upCamera();
    void downCamera();
    void strafeLeftCamera();
    void strafeRightCamera();
    void forwardCamera();
    void backwardCamera();

private:
    GCameraData cameraData;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GCAMERA_H
