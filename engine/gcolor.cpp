#include "gcolor.h"

namespace spcGaneshaEngine {
    
GColor GColor::blueColor() {
    return GColor(0.0f, 0.0f, 0.8f);
}

GColor GColor::greenColor() {
    return GColor(0.0f, 1.0f, 0.0f);
}

GColor GColor::redColor() {
    return GColor(1.0f, 0.0f, 0.0f);
}

GColor GColor::whiteColor() {
    return GColor(1.0f, 1.0f, 1.0f);
}

GColor GColor::blackColor() {
    return GColor();
}

GColor::GColor() : red(0.0f), green(0.0f), blue(0.0f) {
}

GColor::GColor(const TFloat _red, const TFloat _green, const TFloat _blue) : red(_red), green(_green), blue(_blue) {
}

GColor::GColor(const GColor& color) {
    red = color.red;
    green = color.green;
    blue = color.blue;
}

GColor::~GColor() {
}
    
}   //  namespace spcGaneshaEngine
