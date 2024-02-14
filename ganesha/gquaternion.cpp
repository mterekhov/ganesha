#include "gquaternion.h"

namespace spcGaneshaEngine {

GQuaternion::GQuaternion() : x(0), y(0), z(0), w(0) {
    
}

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

GQuaternion GQuaternion::euler(const TFloat pitch, const TFloat yaw, const TFloat roll) const {
    TFloat halfPitch = pitch / 2.0f;
    TFloat halfYaw = yaw / 2.0f;
    TFloat halfRoll = roll / 2.0f;

    TFloat cX = cosf(halfPitch);
    TFloat cY = cosf(halfYaw);
    TFloat cZ = cosf(halfRoll);
    TFloat sX = sinf(halfPitch);
    TFloat sY = sinf(halfYaw);
    TFloat sZ = sinf(halfRoll);

    TFloat cYcZ = cY * cZ;
    TFloat sYsZ = sY * sZ;
    TFloat cYsZ = cY * sZ;
    TFloat sYcZ = sY * cZ;

    GQuaternion updatedVersion;
    updatedVersion.w = cX * cYcZ + sX * sYsZ;
    updatedVersion.x = sX * cYcZ - cX * sYsZ;
    updatedVersion.y = cX * sYcZ + sX * cYsZ;
    updatedVersion.z = cX * cYsZ - sX * sYcZ;
    return updatedVersion;
}

TFloat GQuaternion::euler_x() const {
    return atan2f(2.0f * (w * x + y * z), 1.0f - 2.0f * (x * x + y * y));
}

TFloat GQuaternion::euler_y() const {
    TFloat sinValue = 2.0f * (w * y - z * x);
    if (sinValue > 1.0f) {
        return M_PI_2;
    }
    
    if (sinValue < -1.0f) {
        return -M_PI_2;
    }

    return asinf(sinValue);;
}

TFloat GQuaternion::euler_z() const {
    return atan2f(2.0f * (w * z + x * y), 1.0f - 2.0f * (y * y + z * z));
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
