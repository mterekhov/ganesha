#include "gmouseevent.h"

namespace spcGaneshaEngine {

GMouseEvent::GMouseEvent(const TFloat position_x, const TFloat position_y) : GEvent(EVENT_TYPE_MOUSE_MOVED), position_x(position_x), position_y(position_y) {
    
}

GMouseEvent::~GMouseEvent() {
    
}

const std::string GMouseEvent::about() {
    std::string aboutString = "GMouseEvent position_x = ";
    
    aboutString += std::to_string(position_x);
    aboutString += ", position_y = ";
    aboutString += std::to_string(position_y);
    
    return aboutString;
}

}
