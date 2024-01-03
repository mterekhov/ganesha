#include "gcamera.h"
#include <math.h>

namespace spcGaneshaEngine {

GCamera::GCamera() : positionPoint(GPoint(0, -1, 3)), centerPoint(GPoint(0, 0, 0)) {
    updateVectors();
}

GCamera::GCamera(const GPoint& initialPosition, const GPoint& focusPoint) : positionPoint(initialPosition), centerPoint(focusPoint) {
    updateVectors();
}

void GCamera::mouseCamera(const TFloat diff_x, const TFloat diff_y) {
    if (diff_y > 0) {
        rotationAroundVertical += mouseSens * diff_y * M_PI;
        if (rotationAroundVertical > M_PI_2) {
            rotationAroundVertical = M_PI_2;
        }
        if (rotationAroundVertical < 0) {
            rotationAroundVertical = 0.1;
        }
        sightVector.rotate(rotationAroundVertical, worldUpVector);
        sightVector.normalize();
        positionPoint.y = centerPoint.y + sightVector.y;
    }
    
    if (diff_x > 0) {
        rotationAroundStrafe += mouseSens * diff_x * M_PI;
        if (rotationAroundStrafe > M_PI_2) {
            rotationAroundStrafe = M_PI_2;
        }
        if (rotationAroundStrafe < 0) {
            rotationAroundStrafe = 0.1;
        }
        sightVector.rotate(rotationAroundStrafe, strafeVector);
        sightVector.normalize();
        positionPoint.x = centerPoint.x + sightVector.x;
    }

    updateVectors();
}

GMatrix GCamera::viewMatrix() {
    GVector positionVector(positionPoint.x, positionPoint.y, positionPoint.z);
    TFloat xdotEye = strafeVector.dot(positionVector);
    TFloat ydotEye = cameraUpVector.dot(positionVector);
    TFloat zdotEye = sightVector.dot(positionVector);

    GMatrix lookAtMatrix = GMatrix::identityMatrix();
    
    lookAtMatrix.m[0][0] = strafeVector.x;
    lookAtMatrix.m[1][0] = strafeVector.y;
    lookAtMatrix.m[2][0] = strafeVector.z;
    lookAtMatrix.m[3][0] = xdotEye;
    
    lookAtMatrix.m[0][1] = cameraUpVector.x;
    lookAtMatrix.m[1][1] = cameraUpVector.y;
    lookAtMatrix.m[2][1] = cameraUpVector.z;
    lookAtMatrix.m[3][1] = ydotEye;
    
    lookAtMatrix.m[0][2] = sightVector.x;
    lookAtMatrix.m[1][2] = sightVector.y;
    lookAtMatrix.m[2][2] = sightVector.z;
    lookAtMatrix.m[3][2] = -zdotEye;

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

#pragma mark - Routine -

void GCamera::updateVectors() {
    sightVector = GVector(positionPoint.x - centerPoint.x, positionPoint.y - centerPoint.y, positionPoint.z - centerPoint.z);
    sightVector.normalize();
    
    strafeVector = worldUpVector.cross(sightVector);
    strafeVector.normalize();
    
    cameraUpVector = sightVector.cross(strafeVector);

    strafeVector = sightVector.cross(cameraUpVector);
    strafeVector.normalize();
}

}
