#include "gkeyboardevent.h"

namespace spcGaneshaEngine {

GKeyboardEvent::GKeyboardEvent(const TUInt keyCode) : GEvent(EVENT_TYPE_KEY_DOWN), keyCode(keyCode) {
    
}

GKeyboardEvent::~GKeyboardEvent() {
    
}

const std::string GKeyboardEvent::about() {
    std::string aboutString = "GKeyboardEvent keyCode = ";
    
    aboutString += std::to_string(keyCode);

    return aboutString;
}

}
