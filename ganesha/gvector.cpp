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

void GVector::rotate(const TFloat angle, const GVector& aroundVector) {
    TFloat cosinus = cosf(angle);
    TFloat sinus = sinf(angle);

    GVector tmp;
    tmp.x = (cosinus + (1 - cosinus) * x * x) * aroundVector.x;
    tmp.x += ((1 - cosinus) * x * y - z * sinus) * aroundVector.y;
    tmp.x += ((1 - cosinus) * x * z + y * sinus) * aroundVector.z;

    tmp.y = ((1 - cosinus) * x * y + z * sinus)* aroundVector.x;
    tmp.y += (cosinus + (1 - cosinus) * y * y) * aroundVector.y;
    tmp.y += ((1 - cosinus) * y * z - x * sinus) * aroundVector.z;

    tmp.z = ((1 - cosinus) * x * z - y * sinus) * aroundVector.x;
    tmp.z += ((1 - cosinus) * y * z + x * sinus) * aroundVector.y;
    tmp.z += (cosinus + (1 - cosinus) * z * z) * aroundVector.z;

    x = tmp.x;
    y = tmp.y;
    z = tmp.z;
}

}   //  namespace spcGaneshaEngine
