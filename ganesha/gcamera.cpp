#include <math.h>

#include "gcamera.h"

namespace spcGaneshaEngine {

GCamera::GCamera(const GCameraData& cameraData) : cameraData(cameraData) {
}

GCamera::~GCamera() {
    
}

GMatrix GCamera::viewMatrix() {
    GVector zaxis = GVector(cameraData.eyePoint.x - cameraData.targetPoint.x,
                            cameraData.eyePoint.y - cameraData.targetPoint.y,
                            cameraData.eyePoint.z - cameraData.targetPoint.z);
    zaxis.normalize();
    
    GVector xaxis = cameraData.upVector.cross(zaxis);
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

    GVector eyeVector = GVector(cameraData.eyePoint.x, cameraData.eyePoint.y, cameraData.eyePoint.z);

    matrix.m[3][0] = -eyeVector.dot(xaxis);
    matrix.m[3][1] = -eyeVector.dot(yaxis);
    matrix.m[3][2] = -eyeVector.dot(zaxis);

    return matrix;
}

void GCamera::mouseCamera(const TFloat diff_x, const TFloat diff_y) {
    cameraData.targetPoint.x -= diff_x * cameraData.mouseSens;
    cameraData.targetPoint.y -= diff_y * cameraData.mouseSens;
}

void GCamera::upCamera() {
    cameraData.targetPoint.y += cameraData.keyboardSpeed;
    cameraData.eyePoint.y += cameraData.keyboardSpeed;
}

void GCamera::downCamera() {
    cameraData.targetPoint.y -= cameraData.keyboardSpeed;
    cameraData.eyePoint.y -= cameraData.keyboardSpeed;
}

void GCamera::forwardCamera() {
    GVector directionVector = GVector(cameraData.eyePoint.x - cameraData.targetPoint.x,
                                      cameraData.eyePoint.y - cameraData.targetPoint.y,
                                      cameraData.eyePoint.z - cameraData.targetPoint.z);
    directionVector.normalize();
    cameraData.targetPoint.x -= directionVector.x * cameraData.keyboardSpeed;
    cameraData.targetPoint.y -= directionVector.y * cameraData.keyboardSpeed;
    cameraData.targetPoint.z -= directionVector.z * cameraData.keyboardSpeed;
    
    cameraData.eyePoint.x -= directionVector.x * cameraData.keyboardSpeed;
    cameraData.eyePoint.y -= directionVector.y * cameraData.keyboardSpeed;
    cameraData.eyePoint.z -= directionVector.z * cameraData.keyboardSpeed;
}

void GCamera::backwardCamera() {
    GVector directionVector = GVector(cameraData.eyePoint.x - cameraData.targetPoint.x,
                                      cameraData.eyePoint.y - cameraData.targetPoint.y,
                                      cameraData.eyePoint.z - cameraData.targetPoint.z);
    directionVector.normalize();
    cameraData.targetPoint.x += directionVector.x * cameraData.keyboardSpeed;
    cameraData.targetPoint.y += directionVector.y * cameraData.keyboardSpeed;
    cameraData.targetPoint.z += directionVector.z * cameraData.keyboardSpeed;
    
    cameraData.eyePoint.x += directionVector.x * cameraData.keyboardSpeed;
    cameraData.eyePoint.y += directionVector.y * cameraData.keyboardSpeed;
    cameraData.eyePoint.z += directionVector.z * cameraData.keyboardSpeed;
}

void GCamera::strafeLeftCamera() {
    GVector zaxis = GVector(cameraData.eyePoint.x - cameraData.targetPoint.x,
                            cameraData.eyePoint.y - cameraData.targetPoint.y,
                            cameraData.eyePoint.z - cameraData.targetPoint.z);
    zaxis.normalize();
    
    GVector strafeVector = cameraData.upVector.cross(zaxis);
    strafeVector.normalize();

    cameraData.targetPoint.x -= strafeVector.x * cameraData.keyboardSpeed;
    cameraData.targetPoint.y -= strafeVector.y * cameraData.keyboardSpeed;
    cameraData.targetPoint.z -= strafeVector.z * cameraData.keyboardSpeed;
    
    cameraData.eyePoint.x -= strafeVector.x * cameraData.keyboardSpeed;
    cameraData.eyePoint.y -= strafeVector.y * cameraData.keyboardSpeed;
    cameraData.eyePoint.z -= strafeVector.z * cameraData.keyboardSpeed;
}

void GCamera::strafeRightCamera() {
    GVector zaxis = GVector(cameraData.eyePoint.x - cameraData.targetPoint.x,
                            cameraData.eyePoint.y - cameraData.targetPoint.y,
                            cameraData.eyePoint.z - cameraData.targetPoint.z);
    zaxis.normalize();
    
    GVector strafeVector = cameraData.upVector.cross(zaxis);
    strafeVector.normalize();

    cameraData.targetPoint.x += strafeVector.x * cameraData.keyboardSpeed;
    cameraData.targetPoint.y += strafeVector.y * cameraData.keyboardSpeed;
    cameraData.targetPoint.z += strafeVector.z * cameraData.keyboardSpeed;
    
    cameraData.eyePoint.x += strafeVector.x * cameraData.keyboardSpeed;
    cameraData.eyePoint.y += strafeVector.y * cameraData.keyboardSpeed;
    cameraData.eyePoint.z += strafeVector.z * cameraData.keyboardSpeed;
}

}
