#ifndef SPCGANESHAENGINE_GPOINT2D_H
#define SPCGANESHAENGINE_GPOINT2D_H

#include <vector>

#include "ganeshatypes.h"

namespace spcGaneshaEngine {
    
struct GPoint2D {
public:
    GPoint2D();
    GPoint2D(const TFloat x, const TFloat y);
    GPoint2D(const GPoint2D& point);
    ~GPoint2D();

    TFloat x;
    TFloat y;
};

typedef std::vector<GPoint2D> TPoints2DList;

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GPOINT2D_H
