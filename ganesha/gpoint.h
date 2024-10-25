#ifndef SPCGANESHAENGINE_GPOINT_H
#define SPCGANESHAENGINE_GPOINT_H

#include <vector>

#include "ganeshatypes.h"

namespace spcGaneshaEngine {

struct GPoint {
public:
    GPoint();
    GPoint(const TFloat x, const TFloat y, const TFloat z);
    GPoint(const GPoint& point);
    ~GPoint();

    TFloat x;
    TFloat y;
    TFloat z;
};

typedef std::vector<GPoint> TPointsList;

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GPOINT_H
