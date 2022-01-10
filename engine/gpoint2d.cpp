#include "gpoint2d.h"

namespace spcGaneshaEngine {
    
GPoint2D::GPoint2D() : x(0.0f), y(0.0f) {
}

GPoint2D::GPoint2D(const TFloat x, const TFloat y) : x(x), y(y) {
}

GPoint2D::GPoint2D(const GPoint2D& point) {
    x = point.x;
    y = point.y;
}

GPoint2D::~GPoint2D() {
}

}   //  namespace spcGaneshaEngine
