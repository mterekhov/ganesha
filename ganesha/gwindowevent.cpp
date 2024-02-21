#include "gwindowevent.h"

namespace spcGaneshaEngine {

GWindowEvent::GWindowEvent(const TFloat width, const TFloat height) : GEvent(EVENT_TYPE_WINDOW_RESIZE), width(width), height(height) {
    
}

GWindowEvent::~GWindowEvent() {
    
}

const std::string GWindowEvent::about() {
    std::string aboutString = "GWindowEvent new window size(";
    
    aboutString += std::to_string(width);
    aboutString += " : ";
    aboutString += std::to_string(height);
    
    return aboutString;
}

}
