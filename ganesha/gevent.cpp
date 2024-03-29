#include "gevent.h"

namespace spcGaneshaEngine {

GEvent::GEvent(const EEventType eventType) : eventType(eventType) {
    
}

GEvent::~GEvent() {
    
}

const EEventType GEvent::getType() const {
    return eventType;
}

const std::string GEvent::about() {
    return "empty base class event";
}

}
