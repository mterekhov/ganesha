#include "gupdateframesizeevent.h"

namespace spcGaneshaEngine {

GUpdateFrameSizeEvent::GUpdateFrameSizeEvent(const TFloat width, const TFloat height) : GEvent(EVENT_TYPE_VULKAN_UPDATE_FRAME_SIZE), width(width), height(height) {
    
}

GUpdateFrameSizeEvent::~GUpdateFrameSizeEvent() {
    
}

const std::string GUpdateFrameSizeEvent::about() {
    std::string aboutString = "GUpdateFrameSizeEvent new frame size(";
    
    aboutString += std::to_string(width);
    aboutString += " : ";
    aboutString += std::to_string(height);
    
    return aboutString;
}

}