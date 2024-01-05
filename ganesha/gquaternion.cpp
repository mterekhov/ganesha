#include "gquaternion.h"

namespace spcGaneshaEngine {

GQuaternion::GQuaternion(const GVector& axis, const TFloat angle) {
    GVector normalizedAxis(axis);
    
    normalizedAxis.normalize();
    
    TFloat halfAngle = angle / 2.0f;
    
    TFloat sinus = sinf(halfAngle);
    TFloat cosinus = cosf(halfAngle);

    x = normalizedAxis.x * sinus;
    y = normalizedAxis.y * sinus;
    z = normalizedAxis.z * sinus;
    w = cosinus;
}

GQuaternion::~GQuaternion() {
}

const GQuaternion& GQuaternion::operator*=(const GQuaternion &q) {
    TFloat tx = w * q.x + x * q.w + y * q.z - z * q.y;
    TFloat ty = w * q.y + y * q.w + z * q.x - x * q.z;
    TFloat tz = w * q.z + z * q.w + x * q.y - y * q.x;
    w = w * q.w - x * q.x - y * q.y - z * q.z;
    
    x = tx; 
    y = ty;
    z = tz;
    
    return (*this);
}

GMatrix GQuaternion::matrix() const {
    TFloat x2 = x + x;
    TFloat y2 = y + y;
    TFloat z2 = z + z;
    
    TFloat xx = x * x2;
    TFloat xy = x * y2;
    TFloat xz = x * z2;
    TFloat yy = y * y2;
    TFloat yz = y * z2;
    TFloat zz = z * z2;
    TFloat wx = w * x2;
    TFloat wy = w * y2;
    TFloat wz = w * z2;
    
    GMatrix result;
    
    result.m[0][0] = 1.0 - (yy + zz);
    result.m[1][0] = xy - wz;
    result.m[2][0] = xz + wy;
    result.m[3][0] = 0.0;
    
    result.m[0][1] = xy + wz;
    result.m[1][1] = 1.0 - (xx + zz);
    result.m[2][1] = yz - wx;
    result.m[3][1] = 0.0;
    
    result.m[0][2] = xz - wy;
    result.m[1][2] = yz + wx;
    result.m[2][2] = 1.0 - (xx + yy);
    result.m[3][2] = 0.0;
    
    result.m[0][3] = 0.0;
    result.m[1][3] = 0.0;
    result.m[2][3] = 0.0;
    result.m[3][3] = 1.0;
    
    return result;
}

}   //  namespace spcGaneshaEngine
