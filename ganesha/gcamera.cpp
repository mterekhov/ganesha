#include "gcamera.h"
#include <math.h>

namespace spcGaneshaEngine {

GCamera::GCamera() : positionPoint(GPoint(0, -1, 10)) {
}

GCamera::~GCamera() {
    
}


TFloat GCamera::aroundX(const TFloat diff, const TFloat currentAngle) {
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

TFloat GCamera::aroundY(const TFloat diff, const TFloat currentAngle) {
    TFloat angle = mouseSens * M_PI;
    if (diff < 0) {
        angle = -angle;
    }

    return currentAngle + angle;
}

void GCamera::mouseCamera(const TFloat diff_x, const TFloat diff_y) {
    TFloat around_x = aroundX(diff_y, orientation.euler_x());
    TFloat around_y = aroundY(diff_x, orientation.euler_y());
    TFloat around_z = orientation.euler_z();
    
    orientation.euler(around_x, around_y, around_z);
}

GMatrix GCamera::viewMatrix() {
    GMatrix lookAtMatrix = orientation.matrix();
    lookAtMatrix.m[3][0] = positionPoint.x;
    lookAtMatrix.m[3][1] = positionPoint.y;
    lookAtMatrix.m[3][2] = -positionPoint.z;

    return lookAtMatrix;
}

void GCamera::upCamera() {
    positionPoint.y += keyboardSpeed;
}

void GCamera::downCamera() {
    positionPoint.y -= keyboardSpeed;
}

void GCamera::strafeLeftCamera() {
    positionPoint.x += keyboardSpeed;
}

void GCamera::strafeRightCamera() {
    positionPoint.x -= keyboardSpeed;
}

void GCamera::forwardCamera() {
    positionPoint.z -= keyboardSpeed;
}

void GCamera::backwardCamera() {
    positionPoint.z += keyboardSpeed;
}

}
