#include "gwindowresizeevent.h"

namespace spcGaneshaEngine {

GWindowResizeEvent::GWindowResizeEvent(const TFloat width, const TFloat height) : GEvent(EVENT_TYPE_WINDOW_RESIZED), width(width), height(height) {
    
}

GWindowResizeEvent::~GWindowResizeEvent() {
    
}

const std::string GWindowResizeEvent::about() {
    std::string aboutString = "GWindowResizeEvent width = ";
    
    aboutString += std::to_string(width);
    aboutString += ", height = ";
    aboutString += std::to_string(height);
    
    return aboutString;
}

}
