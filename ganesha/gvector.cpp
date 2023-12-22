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
    TFloat len = x * x + y * y + z * z;
    if (len > FLOAT_PRECISION) {
        x /= sqrtf(len);
        y /= sqrtf(len);
        z /= sqrtf(len);
    }
}

GVector GVector::cross(const GVector& vector) const {
    GVector crossProduct;

    crossProduct.x = vector.z * y - z * vector.y;
    crossProduct.y = vector.x * z - x * vector.z;
    crossProduct.z = vector.x * y - x * vector.y;

    return crossProduct;
}

TFloat GVector::dot(const GVector& vector) const {
    return x * vector.x + y * vector.y + z * vector.z;
}

}   //  namespace spcGaneshaEngine
