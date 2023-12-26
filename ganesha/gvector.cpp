#include <math.h>

#include "gvector.h"

namespace spcGaneshaEngine {

GVector::GVector() : x(0.0f), y(0.0f), z(0.0f) {
}

GVector::GVector(const TFloat x, const TFloat y, const TFloat z) : x(x), y(y), z(z) {
}

GVector::GVector(const GVector& vector) {
    x = vector.x;
    y = vector.y;
    z = vector.z;
}

GVector::~GVector() {
}

void GVector::normalize() {
    TFloat len = magnitude();
    if (len > FLOAT_PRECISION) {
        x /= len;
        y /= len;
        z /= len;
    }
}

GVector GVector::cross(const GVector& vector) const {
    GVector crossProduct;
    
    crossProduct.x = vector.z * y - z * vector.y;
    crossProduct.y = vector.x * z - x * vector.z;
    crossProduct.z = vector.y * x - y * vector.x;
    
    return crossProduct;
}

TFloat GVector::dot(const GVector& vector) const {
    return x * vector.x + y * vector.y + z * vector.z;
}

TFloat GVector::magnitude() const {
    return sqrtf(x * x + y * y + z * z);
}

TFloat GVector::angleBetween(GVector vector) {
    TFloat vectorsMagnitude = magnitude() * vector.magnitude();
    TFloat angle = acosf(dot(vector) / vectorsMagnitude);
    
    return angle;
}

GVector GVector::rotate(const TFloat angle, const GVector& vector) {
    TFloat cosinus = cosf(angle);
    TFloat sinus = sinf(angle);

    GVector tmp;
    tmp.x = (cosinus + (1 - cosinus) * vector.x * vector.x) * x;
    tmp.x += ((1 - cosinus) * vector.x * vector.y - vector.z * sinus) * y;
    tmp.x += ((1 - cosinus) * vector.x * vector.z + vector.y * sinus) * z;

    tmp.y = ((1 - cosinus) * vector.x * vector.y + vector.z * sinus)* x;
    tmp.y += (cosinus + (1 - cosinus) * vector.y * vector.y) * y;
    tmp.y += ((1 - cosinus) * vector.y * vector.z - vector.x * sinus) * z;

    tmp.z = ((1 - cosinus) * vector.x * vector.z - vector.y * sinus) * x;
    tmp.z += ((1 - cosinus) * vector.y * vector.z + vector.x * sinus) * y;
    tmp.z += (cosinus + (1 - cosinus) * vector.z * vector.z) * z;

    return tmp;
}

}   //  namespace spcGaneshaEngine
