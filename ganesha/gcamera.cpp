#include <math.h>
#include "gcamera.h"

namespace spcGaneshaEngine {

GCamera::GCamera() {
}

GCamera::~GCamera() {
    
}

GQuaternion GCamera::mouseCamera(const GQuaternion& orientation, const TFloat diff_x, const TFloat diff_y, const TFloat mouseSens) {
    TFloat around_x = aroundX(diff_y, orientation.euler_x(), mouseSens);
    TFloat around_y = aroundY(diff_x, orientation.euler_y(), mouseSens);
    TFloat around_z = orientation.euler_z();
    
    return orientation.euler(around_x, around_y, around_z);
}

GMatrix GCamera::viewMatrix(const GQuaternion& orientation, const GPoint& positionPoint) {
    GMatrix lookAtMatrix = orientation.matrix();
    lookAtMatrix.m[3][0] = positionPoint.x;
    lookAtMatrix.m[3][1] = positionPoint.y;
    lookAtMatrix.m[3][2] = -positionPoint.z;

    return lookAtMatrix;
}

GPoint GCamera::upCamera(const GPoint& positionPoint, const TFloat keyboardSpeed) {
    GPoint updated = positionPoint;
    updated.y += keyboardSpeed;
    return updated;
}

GPoint GCamera::downCamera(const GPoint& positionPoint, const TFloat keyboardSpeed) {
    GPoint updated = positionPoint;
    updated.y -= keyboardSpeed;
    return updated;
}

GPoint GCamera::strafeLeftCamera(const GPoint& positionPoint, const TFloat keyboardSpeed) {
    GPoint updated = positionPoint;
    updated.x += keyboardSpeed;
    return updated;
}

GPoint GCamera::strafeRightCamera(const GPoint& positionPoint, const TFloat keyboardSpeed) {
    GPoint updated = positionPoint;
    updated.x -= keyboardSpeed;
    return updated;
}

GPoint GCamera::forwardCamera(const GPoint& positionPoint, const TFloat keyboardSpeed) {
    GPoint updated = positionPoint;
    updated.z -= keyboardSpeed;
    return updated;
}

GPoint GCamera::backwardCamera(const GPoint& positionPoint, const TFloat keyboardSpeed) {
    GPoint updated = positionPoint;
    updated.z += keyboardSpeed;
    return updated;
}

TFloat GCamera::aroundX(const TFloat diff, const TFloat currentAngle, const TFloat mouseSens) {
    TFloat angle = mouseSens * M_PI;
    if (diff < 0) {
        angle *= -1;
    }

    TFloat resultAngle = currentAngle + angle;
    if (resultAngle > M_PI_2) {
        resultAngle = M_PI_2;
    }
    if (resultAngle < 0) {
        resultAngle = 0;
    }
    
    return resultAngle;
}

TFloat GCamera::aroundY(const TFloat diff, const TFloat currentAngle, const TFloat mouseSens) {
    TFloat angle = mouseSens * M_PI;
    if (diff < 0) {
        angle = -angle;
    }

    return currentAngle + angle;
}

}
