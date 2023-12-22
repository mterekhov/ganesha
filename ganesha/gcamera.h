#ifndef SPCGANESHAENGINE_GCAMERA_H
#define SPCGANESHAENGINE_GCAMERA_H

#include "ganeshatypes.h"
#include "gvector.h"
#include "gmatrix.h"

namespace spcGaneshaEngine {

/// Manipulates with current position
class GCamera {
private:
    GPoint position;
    GPoint center;
    GVector up;

    TFloat mSens;
    TFloat kSpeed;
    GVector strafe;

public:
    GCamera();
    GCamera(const GPoint& newPosition, const GPoint& newCenter, const GVector& newUp);
    GMatrix viewMatrix() const;

    void moveCamera(TFloat speed);
    void updownCamera(TFloat speed);
    void strafeCamera(TFloat speed);

//    void rotateCamera(TFloat angle, TFloat x,  TFloat y,  TFloat z);
//    void mouseCamera();
//    void setupCamera();
//
//    GVector Cross(GVector vector_1, GVector vector_2);
//
//    GVector Normalize(GVector vector);
//
//    TFloat vectorLength(GVector vector);
//
//    TFloat angleBetweenVectors(GVector vector_1, GVector vector_2);
//
//    TFloat scalMult(GVector vector_1, GVector vector_2);
//
//    GVector rotateVector(TFloat angle, GVector vector, TFloat x, TFloat y, TFloat z);
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GCAMERA_H
