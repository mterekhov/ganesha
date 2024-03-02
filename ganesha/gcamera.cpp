#include <math.h>
#include "gcamera.h"

namespace spcGaneshaEngine {

GCamera::GCamera() {
}

GCamera::~GCamera() {
    
}

GMatrix GCamera::viewMatrix(const GPoint& eyePoint, const GPoint& targetPoint, const GVector& upVector) {
    GVector zaxis = GVector(eyePoint.x - targetPoint.x, eyePoint.y - targetPoint.y, eyePoint.z - targetPoint.z);
    zaxis.normalize();
    
    GVector xaxis = upVector.cross(zaxis);
    xaxis.normalize();

    GVector yaxis = zaxis.cross(xaxis);
    
    GMatrix matrix = GMatrix::identityMatrix();
    
    matrix.m[0][0] = xaxis.x;
    matrix.m[0][1] = yaxis.x;
    matrix.m[0][2] = zaxis.x;

    matrix.m[1][0] = xaxis.y;
    matrix.m[1][1] = yaxis.y;
    matrix.m[1][2] = zaxis.y;

    matrix.m[2][0] = xaxis.z;
    matrix.m[2][1] = yaxis.z;
    matrix.m[2][2] = zaxis.z;

    GVector eyeVector = GVector(eyePoint.x, eyePoint.y, eyePoint.z);

    matrix.m[3][0] = -eyeVector.dot(xaxis);
    matrix.m[3][1] = -eyeVector.dot(yaxis);
    matrix.m[3][2] = -eyeVector.dot(zaxis);

    return matrix;
}

GPoint GCamera::mouseCamera(const GPoint& targetPoint, const TFloat diff_x, const TFloat diff_y, const TFloat mouseSens) {
    GPoint newPoint = targetPoint;

    newPoint.x -= diff_x * mouseSens;
    newPoint.y -= diff_y * mouseSens;

    return newPoint;
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
