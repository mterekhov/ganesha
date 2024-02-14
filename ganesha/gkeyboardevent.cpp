#include "gkeyboardevent.h"

namespace spcGaneshaEngine {

GKeyboardEvent::GKeyboardEvent(const TUInt keyCode) : GEvent(EVENT_TYPE_KEY_DOWN), keyCode(keyCode) {
    
}

GKeyboardEvent::~GKeyboardEvent() {
    
}

}
