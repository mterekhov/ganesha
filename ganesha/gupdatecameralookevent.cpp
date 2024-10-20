#include "gupdatecameralookevent.h"

namespace spcGaneshaEngine {

GUpdateCameraLookEvent::GUpdateCameraLookEvent(const GPoint2D& lookUpdate) : GEvent(EVENT_TYPE_VULKAN_CAMERA_LOOK_UPDATE), lookUpdate(lookUpdate) {
}

GUpdateCameraLookEvent::~GUpdateCameraLookEvent() {
    
}

const std::string GUpdateCameraLookEvent::about() {
    std::string aboutString = "GUpdateCameraLookEvent camera look update: x = ";

    aboutString += std::to_string(lookUpdate.x);
    aboutString += ", y = ";
    aboutString += std::to_string(lookUpdate.y);

    return aboutString;
}

}
