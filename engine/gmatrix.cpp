#include "gmatrix.h"
#include <math.h>

namespace spcGaneshaEngine  {

GMatrix GMatrix::identityMatrix() {
    GMatrix m;
    m.m[0][0] = 1.0;
    m.m[1][0] = 0.0;
    m.m[2][0] = 0.0;
    m.m[3][0] = 0.0;
    
    m.m[0][1] = 0.0;
    m.m[1][1] = 1.0;
    m.m[2][1] = 0.0;
    m.m[3][1] = 0.0;
    
    m.m[0][2] = 0.0;
    m.m[1][2] = 0.0;
    m.m[2][2] = 1.0;
    m.m[3][2] = 0.0;
    
    m.m[0][3] = 0.0;
    m.m[1][3] = 0.0;
    m.m[2][3] = 0.0;
    m.m[3][3] = 1.0;
    
    return m;
}

GMatrix GMatrix::zeroMatrix() {
    GMatrix m;
    m.m[0][0] = 0.0;
    m.m[1][0] = 0.0;
    m.m[2][0] = 0.0;
    m.m[3][0] = 0.0;
    
    m.m[0][1] = 0.0;
    m.m[1][1] = 0.0;
    m.m[2][1] = 0.0;
    m.m[3][1] = 0.0;
    
    m.m[0][2] = 0.0;
    m.m[1][2] = 0.0;
    m.m[2][2] = 0.0;
    m.m[3][2] = 0.0;
    
    m.m[0][3] = 0.0;
    m.m[1][3] = 0.0;
    m.m[2][3] = 0.0;
    m.m[3][3] = 0.0;
    
    return m;
}

void GMatrix::transponate() {
    TFloat tmp[4][4];
    memcpy(tmp, m, sizeof(TFloat) * 16);

    m[0][1] = tmp[1][0];
    m[0][2] = tmp[2][0];
    m[0][3] = tmp[3][0];

    m[1][0] = tmp[0][1];
    m[1][2] = tmp[2][1];
    m[1][3] = tmp[3][1];

    m[2][0] = tmp[0][2];
    m[2][1] = tmp[1][2];
    m[2][3] = tmp[3][2];

    m[3][0] = tmp[0][3];
    m[3][1] = tmp[1][3];
    m[3][2] = tmp[2][3];
}

GMatrix GMatrix::frustum(const TFloat left,
                         const TFloat right,
                         const TFloat bottom,
                         const TFloat top,
                         const TFloat near,
                         const TFloat far) {
    TFloat a = (right + left) / (right - left);
    TFloat b = (top + bottom) / (top - bottom);
    TFloat c = - (far + near) / (far - near);
    TFloat d = - (2.0 * far * near) / (far - near);

    GMatrix frustumMatrix = GMatrix::zeroMatrix();
    frustumMatrix.m[0][0] = 2.0 * near / (right - left);
    frustumMatrix.m[0][1] = 0.0f;
    frustumMatrix.m[0][2] = 0.0f;
    frustumMatrix.m[0][3] = 0.0f;
    
    frustumMatrix.m[1][0] = 0.0f;
    frustumMatrix.m[1][1] = -2.0f * near / (top - bottom);
    frustumMatrix.m[1][2] = 0.0f;
    frustumMatrix.m[1][3] = 0.0f;
    
    frustumMatrix.m[2][0] = a;
    frustumMatrix.m[2][1] = b;
    frustumMatrix.m[2][2] = c;
    frustumMatrix.m[2][3] = -1.0f;
    
    frustumMatrix.m[3][0] = 0.0f;
    frustumMatrix.m[3][1] = 0.0f;
    frustumMatrix.m[3][2] = d;
    frustumMatrix.m[3][3] = 0.0f;

    return frustumMatrix;
}

GMatrix GMatrix::lookAt(const GVector& eye, const GVector& center, const GVector& up) {
    GVector zaxis(eye.x - center.x, eye.y - center.y, eye.z - center.z);
    zaxis.normalize();
    
    GVector xaxis = zaxis;
    xaxis = up.cross(xaxis);
    xaxis.normalize();
    
    GVector yaxix = zaxis.cross(xaxis);
    yaxix.z = -yaxix.z;
    
    TFloat xdotEye = xaxis.dot(eye);
    TFloat ydotEye = yaxix.dot(eye);
    TFloat zdotEye = zaxis.dot(eye);

    GMatrix lookAtMatrix = GMatrix::zeroMatrix();
    lookAtMatrix.m[0][0] = xaxis.x;
    lookAtMatrix.m[0][1] = xaxis.y;
    lookAtMatrix.m[0][2] = xaxis.z;
    lookAtMatrix.m[0][3] = xdotEye;
    
    lookAtMatrix.m[1][0] = yaxix.x;
    lookAtMatrix.m[1][1] = yaxix.y;
    lookAtMatrix.m[1][2] = yaxix.z;
    lookAtMatrix.m[1][3] = ydotEye;
    
    lookAtMatrix.m[2][0] = zaxis.x;
    lookAtMatrix.m[2][1] = zaxis.y;
    lookAtMatrix.m[2][2] = zaxis.z;
    lookAtMatrix.m[2][3] = -zdotEye;
    
    lookAtMatrix.m[3][0] = 0.0f;
    lookAtMatrix.m[3][1] = 0.0f;
    lookAtMatrix.m[3][2] = 0.0f;
    lookAtMatrix.m[3][3] = 1.0f;
    
    lookAtMatrix.transponate();
    return lookAtMatrix;
}

GMatrix GMatrix::rotationX(const TFloat angle) {
	TFloat sine = sinf(angle);
	TFloat cosine = cosf(angle);
	
    GMatrix m;
	m.m[0][0] = 1.0;
	m.m[1][0] = 0.0;
	m.m[2][0] = 0.0;
	m.m[3][0] = 0.0;
	
	m.m[0][1] = 0.0;
	m.m[1][1] = (cosine);
	m.m[2][1] = (sine);
	m.m[3][1] = 0.0;
	
	m.m[0][2] = 0.0;
	m.m[1][2] = -(sine);
	m.m[2][2] = (cosine);
	m.m[3][2] = 0.0;
	
	m.m[0][3] = 0.0;
	m.m[1][3] = 0.0;
	m.m[2][3] = 0.0;
	m.m[3][3] = 1.0;
	
	return m;
}

GMatrix GMatrix::rotationY(const TFloat angle) {
	TFloat sine = sinf(angle);
	TFloat cosine = cosf(angle);
	
    GMatrix m;
	m.m[0][0] = (cosine);
	m.m[1][0] = 0.0;
	m.m[2][0] = -(sine);
	m.m[3][0] = 0.0;
	
	m.m[0][1] = 0.0;
	m.m[1][1] = 1.0;
	m.m[2][1] = 0.0;
	m.m[3][1] = 0.0;
	
	m.m[0][2] = (sine);
	m.m[1][2] = 0.0;
	m.m[2][2] = (cosine);
	m.m[3][2] = 0.0;
	
	m.m[0][3] = 0.0;
	m.m[1][3] = 0.0;
	m.m[2][3] = 0.0;
	m.m[3][3] = 1.0;
	
	return m;
}

GMatrix GMatrix::rotationZ(const TFloat angle)
{
	TFloat sine = sinf(angle);
	TFloat cosine = cosf(angle);
	
    GMatrix m;
	m.m[0][0] = (cosine);
	m.m[1][0] = (sine);
	m.m[2][0] = 0.0;
	m.m[3][0] = 0.0;
	
	m.m[0][1] = -(sine);
	m.m[1][1] = (cosine);
	m.m[2][1] = 0.0;
	m.m[3][1] = 0.0;
	
	m.m[0][2] = 0.0;
	m.m[1][2] = 0.0;
	m.m[2][2] = 1.0;
	m.m[3][2] = 0.0;
	
	m.m[0][3] = 0.0;
	m.m[1][3] = 0.0;
	m.m[2][3] = 0.0;
	m.m[3][3] = 1.0;
	
	return m;
}

}   //  namespace spcGaneshaEngine
