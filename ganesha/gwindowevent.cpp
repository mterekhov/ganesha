#include "gwindowevent.h"

namespace spcGaneshaEngine {

GWindowEvent::GWindowEvent(const TFloat width, const TFloat height) : GEvent(EVENT_TYPE_WINDOW_RESIZE), width(width), height(height) {
    
}

GWindowEvent::~GWindowEvent() {
    
}

}
