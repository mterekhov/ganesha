#include "gpoint.h"

namespace spcGaneshaEngine {

GPoint::GPoint() : x(0.0f), y(0.0f), z(0.0f) {
}

GPoint::GPoint(const TFloat x, const TFloat y, const TFloat z) : x(x), y(y), z(z) {
}

GPoint::GPoint(const GPoint& point) {
    x = point.x;
    y = point.y;
    z = point.z;
}

GPoint::~GPoint() {
}

}   //  namespace spcGaneshaEngine
