#include "gcamera.h"
#include <math.h>

namespace spcGaneshaEngine {

GCamera::GCamera() : positionPoint(GPoint(0, -1, 3)), centerPoint(GPoint(0, 0, 0)) {
}

GCamera::GCamera(const GPoint& initialPosition, const GPoint& focusPoint) : positionPoint(initialPosition), centerPoint(focusPoint) {
}

void GCamera::mouseCamera(const TFloat diff_x, const TFloat diff_y) {
    TFloat rotationAngle = mouseSens * diff_x * M_PI_2;
//    sightVector = strafeVector.rotate(rotationAngle, sightVector);
//    sightVector.normalize();

    rotationAngle = mouseSens * diff_x * M_PI_2;
    if (rotationAngle > M_PI_2) {
        rotationAngle = M_PI_2;
    }
    if (rotationAngle < -M_PI_2) {
        rotationAngle = -M_PI_2;
    }

    sightVector = sightVector.rotate(rotationAngle, cameraUpVector);
    sightVector.normalize();

    centerPoint.x = positionPoint.x + sightVector.x;
    centerPoint.y = positionPoint.y + sightVector.y;
//    centerPoint.z = positionPoint.z + sightVector.z;

//    centerPoint = GPoint(positionPoint.x + sightVector.x,
//                         positionPoint.y + sightVector.y,
//                         positionPoint.z + sightVector.z);
}

GMatrix GCamera::viewMatrix() {
    sightVector = GVector(positionPoint.x - centerPoint.x, positionPoint.y - centerPoint.y, positionPoint.z - centerPoint.z);
    sightVector.normalize();
    
    strafeVector = worldUpVector.cross(sightVector);
    strafeVector.normalize();
    
    cameraUpVector = sightVector.cross(strafeVector);

    strafeVector = sightVector.cross(cameraUpVector);
    strafeVector.normalize();

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
    positionPoint.y -= keyboardSpeed;
    centerPoint.y -= keyboardSpeed;
}

void GCamera::downCamera() {
    positionPoint.y += keyboardSpeed;
    centerPoint.y += keyboardSpeed;
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

//void GCamera::moveCamera(TFloat speed) {
//    GVector tmp;
//    TFloat angle;
//
//    tmp.x = view.x - position.x;
//    tmp.y = view.y - position.y;
//    tmp.z = view.z - position.z;
//
//    angle = (TFloat)(angleBetweenVectors(tmp, up) - 90);
//
//    tmp = rotateVector(angle, tmp, strafe.x, strafe.y, strafe.z);
//
//    position.x += tmp.x * speed;
//    position.z += tmp.z * speed;
//
//    view.x += tmp.x * speed;
//    view.z += tmp.z * speed;
//}
//
//void GCamera::rotateCamera(TFloat angle, TFloat x,  TFloat y,  TFloat z) {
//	GVector tmp;
//
//	tmp.x = view.x - position.x;
//	tmp.y = view.y - position.y;
//	tmp.z = view.z - position.z;
//
//	tmp = rotateVector(angle, tmp, x, y, z);
//
//	view.x = position.x + tmp.x;
//	view.y = position.y + tmp.y;
//	view.z = position.z + tmp.z;
//}
//
//void GCamera::mouseCamera() {
//	POINT mousePoint;
//	int centerX, centerY;
//	TFloat angleY, angleZ;
//	static TFloat currentRotX = 0, lastRotX = 0;
//	GVector normalAxis, tmp;
//
//	tmp.x = view.x - position.x;
//	tmp.y = view.y - position.y;
//	tmp.z = view.z - position.z;
//
//	centerX = sets.wndLeft + (sets.wndWidth >> 1);
//	centerY = sets.wndTop + (sets.wndHeight >> 1);
//	GetCursorPos(&mousePoint);
//
//	if ((mousePoint.x == centerX) && (mousePoint.y == centerY)) return;
//	SetCursorPos(centerX, centerY);
//
//	angleY = (centerX - mousePoint.x) / mSens;
//	angleZ = (centerY - mousePoint.y) / mSens;
//
//	lastRotX = currentRotX;
//
//	currentRotX += angleZ;
//
//	if (currentRotX > 1)
//	{
//		currentRotX = 1;
//		if (lastRotX != 1)
//		{
//			normalAxis = Normalize(Cross(tmp, up));
//			rotateCamera(1 - lastRotX, normalAxis.x, normalAxis.y, normalAxis.z);
//		}
//	}
//	else
//	{
//		if (currentRotX < -1)
//		{
//			currentRotX = -1;
//			if (lastRotX != -1)
//			{
//				normalAxis = Normalize(Cross(tmp, up));
//				rotateCamera(-1 - lastRotX, normalAxis.x, normalAxis.y, normalAxis.z);
//			}
//		}
//		else
//		{
//			normalAxis = Normalize(Cross(tmp, up));
//			rotateCamera(angleZ, normalAxis.x, normalAxis.y, normalAxis.z);
//		}
//	}
//
//	rotateCamera(angleY, 0, 1, 0);
//}
//
//void GCamera::strafeCamera(TFloat speed) {
//	position.x += strafe.x * speed;
//	position.z += strafe.z * speed;
//	view.x += strafe.x * speed;
//	view.z += strafe.z * speed;
//}
//
//void GCamera::setupCamera() {
//	GVector tmp;
//
//	tmp.x = view.x - position.x;
//	tmp.y = view.y - position.y;
//	tmp.z = view.z - position.z;
//
//	strafe = Normalize(Cross(tmp, up));
//
//	if (GetKeyState('W') & 0x80) moveCamera(kSpeed);
//	if (GetKeyState('S') & 0x80) moveCamera(-kSpeed);
//
//	if (GetKeyState('Q') & 0x80) updownCamera(kSpeed);
//	if (GetKeyState('E') & 0x80) updownCamera(-kSpeed);
//
//	if (GetKeyState('D') & 0x80) strafeCamera(kSpeed);
//	if (GetKeyState('A') & 0x80) strafeCamera(-kSpeed);
//
//	if (GetKeyState('Z') & 0x80) rotateCamera(kSpeed, 0, 1, 0);
//	if (GetKeyState('X') & 0x80) rotateCamera(-kSpeed, 0, 1, 0);
//
//	mouseCamera();
//}
//
//TFloat GCamera::angleBetweenVectors(GVector vector_1, GVector vector_2) {
//    TFloat angle;
//	TFloat dotProduct, vectorsMagnitude;
//
//	dotProduct = scalMult(vector_1, vector_2);
//	vectorsMagnitude = vectorLength(vector_1) * vectorLength(vector_2);
//	angle = acos(dotProduct / vectorsMagnitude);
//
//	if (_isnan(angle)) return 0;
//	return angle;
//}
//
//GVector GCamera::rotateVector(TFloat angle, GVector vector, TFloat x, TFloat y, TFloat z) {
//	GVector tmp;
//	TFloat cosinus, sinus;
//
//	cosinus = cosf(angle);
//	sinus = sinf(angle);
//
//	tmp.x = (cosinus + (1 - cosinus) * x * x) * vector.x;
//	tmp.x += ((1 - cosinus) * x * y - z * sinus) * vector.y;
//	tmp.x += ((1 - cosinus) * x * z + y * sinus) * vector.z;
//
//	tmp.y = ((1 - cosinus) * x * y + z * sinus)* vector.x;
//	tmp.y += (cosinus + (1 - cosinus) * y * y) * vector.y;
//	tmp.y += ((1 - cosinus) * y * z - x * sinus) * vector.z;
//
//	tmp.z = ((1 - cosinus) * x * z - y * sinus) * vector.x;
//	tmp.z += ((1 - cosinus) * y * z + x * sinus) * vector.y;
//	tmp.z += (cosinus + (1 - cosinus) * z * z) * vector.z;
//
//	return tmp;
//}

}
