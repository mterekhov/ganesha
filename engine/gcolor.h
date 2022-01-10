#ifndef SPCGANESHAENGINE_GCOLOR_H
#define SPCGANESHAENGINE_GCOLOR_H

#include "ganeshatypes.h"

namespace spcGaneshaEngine {

struct GColor {
public:
    static GColor whiteColor();
    static GColor blackColor();
    static GColor redColor();
    static GColor greenColor();
    static GColor blueColor();

    GColor();
    GColor(const TFloat _red, const TFloat _green, const TFloat _blue);
    GColor(const GColor& color);
    ~GColor();

    TFloat red;
    TFloat green;
    TFloat blue;
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GCOLOR_H
