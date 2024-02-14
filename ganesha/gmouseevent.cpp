#include "gmouseevent.h"

namespace spcGaneshaEngine {

GMouseEvent::GMouseEvent(const TFloat position_x, const TFloat position_y) : GEvent(EVENT_TYPE_MOUSE_MOVED), position_x(position_x), position_y(position_y) {
    
}

GMouseEvent::~GMouseEvent() {
    
}

}
