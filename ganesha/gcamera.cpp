#include "gcamera.h"
#include <math.h>

namespace spcGaneshaEngine {

GCamera::GCamera() : positionPoint(GPoint(0, -1, 3)), centerPoint(GPoint(0, 0, 2)) {
}

void GCamera::mouseCamera(const TFloat diff_x, const TFloat diff_y) {
    TFloat angle = mouseSens * M_PI;
    if (diff_y > 0) {
        angle *= -1;
    }

    TFloat around_x = orientation.euler_x() + angle;
    
    angle = mouseSens * M_PI;
    if (diff_x < 0) {
        angle *= -1;
    }
    TFloat around_y = orientation.euler_y() + angle;
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
    centerPoint.y += keyboardSpeed;
}

void GCamera::downCamera() {
    positionPoint.y -= keyboardSpeed;
    centerPoint.y -= keyboardSpeed;
}

void GCamera::strafeLeftCamera() {
    positionPoint.x += keyboardSpeed;
    centerPoint.x += keyboardSpeed;
}

void GCamera::strafeRightCamera() {
    positionPoint.x -= keyboardSpeed;
    centerPoint.x -= keyboardSpeed;
}

void GCamera::forwardCamera() {
    positionPoint.z -= keyboardSpeed;
    centerPoint.z -= keyboardSpeed;
}

void GCamera::backwardCamera() {
    positionPoint.z += keyboardSpeed;
    centerPoint.z += keyboardSpeed;
}

}
