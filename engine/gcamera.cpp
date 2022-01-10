#include "gcamera.h"
#include <math.h>

namespace spcGaneshaEngine {

GCamera::GCamera() : position(GVector(2, 2, 2)), center(GVector(0, 0, 0)), up(GVector(0, 0, 1)), mSens(100), kSpeed(0.25) {
}

GCamera::GCamera(const GVector& newPosition, const GVector& newCenter, const GVector& newUp) : position(newPosition), center(newCenter), up(newUp), mSens(100), kSpeed(0.25) {
}

GMatrix GCamera::viewMatrix() const {
    return GMatrix::lookAt(position, center, up);
}

void GCamera::moveCamera(TFloat speed) {
//	GVector tmp;
//	TFloat angle;
//
//	tmp.x = view.x - position.x;
//	tmp.y = view.y - position.y;
//	tmp.z = view.z - position.z;
//
//	angle = (TFloat)(angleBetweenVectors(tmp, up) - 90);
//
//	tmp = rotateVector(angle, tmp, strafe.x, strafe.y, strafe.z);
//
//	position.x += tmp.x * speed;
//	position.z += tmp.z * speed;
//
//	view.x += tmp.x * speed;
//	view.z += tmp.z * speed;
}

void GCamera::updownCamera(TFloat speed) {
	position.y += speed;
    center.y += speed;
}

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
//GVector GCamera::Cross(GVector vector_1, GVector vector_2) {
//	GVector normal;
//
//	normal.x = ((vector_1.y * vector_2.z) - (vector_1.z * vector_2.y));
//	normal.y = ((vector_1.z * vector_2.x) - (vector_1.x * vector_2.z));
//	normal.z = ((vector_1.x * vector_2.y) - (vector_1.y * vector_2.x));
//
//	return normal;
//}
//
//GVector GCamera::Normalize(GVector vector) {
//	TFloat length;
//
//	length = vectorLength(vector);
//	vector.x /= length;
//	vector.y /= length;
//	vector.z /= length;
//
//	return vector;
//}
//
//TFloat GCamera::vectorLength(GVector vector) {
//	TFloat length;
//
//	length = sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
//
//	return length;
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
//TFloat GCamera::scalMult(GVector vector_1, GVector vector_2) {
//	return vector_1.x * vector_2.x + vector_1.y * vector_2.y + vector_1.z * vector_2.z;
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
