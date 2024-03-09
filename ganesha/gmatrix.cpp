#include <math.h>

#include "gmatrix.h"

namespace spcGaneshaEngine  {

const std::string GMatrix::print() const {
    char const *format = "%.3f\t\t%.3f\t\t%.3f\t\t%.3f\n%.3f\t\t%.3f\t\t%.3f\t\t%.3f\n%.3f\t\t%.3f\t\t%.3f\t\t%.3f\n%.3f\t\t%.3f\t\t%.3f\t\t%.3f\n";
    char printBuffer[BUFSIZ] = {0};
    
    snprintf(printBuffer, BUFSIZ, format,
             m[0][0], m[0][1], m[0][2], m[0][3],
             m[1][0], m[1][1], m[1][2], m[1][3],
             m[2][0], m[2][1], m[2][2], m[2][3],
             m[3][0], m[3][1], m[3][2], m[3][3]);
    
    return printBuffer;
}

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

GMatrix GMatrix::scale(const TFloat x, const TFloat y, const TFloat z) {
    GMatrix result = GMatrix::identityMatrix();
    
    result.m[0][0] = x;
    result.m[1][1] = y;
    result.m[2][2] = z;
    
    return result;
}

GMatrix GMatrix::translastion(const TFloat x, const TFloat y, const TFloat z) {
    GMatrix result = GMatrix::identityMatrix();
    
    result.m[3][0] = x;
    result.m[3][1] = y;
    result.m[3][2] = z;
    
    return result;

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
