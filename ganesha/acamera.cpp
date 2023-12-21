#include "../inc/acamera.h"

//=============================================================================

extern ASets settings;

//=============================================================================

ACamera::ACamera
		(
			float px, float py, float pz,
			float vx, float vy, float vz,
			float ux, float uy, float uz
		)
{
	AVector initPosition = AVector(px, py, pz);
	AVector initView = AVector(vx, vy, vz);
	AVector initUp = AVector(ux, uy, uz);

	position = initPosition;
	view = initView;
	up = initUp;

	cSpeed = 0.1;
	sens = 10;
}

//=============================================================================

void ACamera::moveCamera(float dir)
{
	AVector vector = view - position;
	vector = Normalize(vector);

	position.x += dir * vector.x * cSpeed;
	position.z += dir * vector.z * cSpeed;

	view.x += dir * vector.x * cSpeed;
	view.z += dir * vector.z * cSpeed;
}

//=============================================================================

void ACamera::posCamera
		(
			float posX, float posY, float posZ,
			float viewX, float viewY, float viewZ,
			float upX, float upY, float upZ
		)
{
	AVector initPosition = AVector(posX, posY, posZ);
	AVector initView = AVector(viewX, viewY, viewZ);
	AVector initUp = AVector(upX, upY, upZ);

	position = initPosition;
	view = initView;
	up = initUp;
}

//=============================================================================

void ACamera::rotateCamera(float dir, float x, float y, float z)
{
	AVector vector;
	AVector newVector;
	float cosinus = cos(dir * cSpeed);
	float sinus = sin(dir * cSpeed);

	vector.x = view.x - position.x;
	vector.y = view.y - position.y;
	vector.z = view.z - position.z;

	newVector.x = (cosinus + (1 - cosinus) * x * x) * vector.x;
	newVector.x += ((1 - cosinus) * x * y - z * sinus) * vector.y;
	newVector.x += ((1 - cosinus) * x * z + y * sinus) * vector.z;

	newVector.y = ((1 - cosinus) * x * y + z * sinus) * vector.x;
	newVector.y += (cosinus + (1 - cosinus) * y * y) * vector.y;
	newVector.y += ((1 - cosinus) * y * z - x * sinus) * vector.z;

	newVector.z = ((1 - cosinus) * x * z - y * sinus) * vector.x;
	newVector.z += ((1 - cosinus) * y * z + x * sinus) * vector.y;
	newVector.z += (cosinus + (1 - cosinus) * z * z) * vector.z;

	view.x = position.x + newVector.x;
	view.y = position.y + newVector.y;
	view.z = position.z + newVector.z;
}

//=============================================================================

float ACamera::Magnitude(AVector normal)
{
	return (float)sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
}

//=============================================================================

AVector ACamera::Normalize(AVector vector)
{
	return vector / Magnitude(vector);
}

//=============================================================================

AVector ACamera::Cross(AVector vector_1, AVector vector_2)
{
	AVector normal;
	normal.x = vector_1.y * vector_2.z - vector_1.z * vector_2.y;
	normal.y = vector_1.z * vector_2.x - vector_1.x * vector_2.z;
	normal.z = vector_1.x * vector_2.y - vector_1.y * vector_2.x;
	return normal;
}

//=============================================================================

void ACamera::setMouseView()
{
	POINT mousePos;
	int screenMiddleX = settings.wndWidth >> 1;
	int screenMiddleY = settings.wndHeight >> 1;
	float angleY = 0;
	float angleZ = 0;
	float currentRotX = 0;
	float lastRotX = 0;

	GetCursorPos(&mousePos);

	if ((mousePos.x == screenMiddleX) && (mousePos.y == screenMiddleY)) return;

	SetCursorPos(screenMiddleX, screenMiddleY);
	angleY = (float)(screenMiddleX - mousePos.x) / sens;
	angleZ = (float)(screenMiddleY - mousePos.y) / sens;

	lastRotX = currentRotX;
	currentRotX += angleZ;
	if (currentRotX > 1) currentRotX = 1;
	else
	{
		if (currentRotX < -1) currentRotX = -1;
		else
		{
			AVector axis = Normalize(Cross(view - position, up));
			rotateCamera(angleZ, axis.x, axis.y, axis.z);
			rotateCamera(angleY, 0, 1, 0);
		}
	}
}

//=============================================================================

void ACamera::upDownCamera(float dir)
{
	position.y += cSpeed * dir;
	view.y += cSpeed * dir;
}

//=============================================================================

void ACamera::strafeCamera(float dir)
{
	strafe = Normalize(Cross(view - position, up));

	position.x += strafe.x * cSpeed * dir;
	position.z += strafe.z * cSpeed * dir;

	view.x += strafe.x * cSpeed * dir;
	view.z += strafe.z * cSpeed * dir;
}
