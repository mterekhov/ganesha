#include "gupdatecamerapositionevent.h"

namespace spcGaneshaEngine {

GUpdateCameraPositionEvent::GUpdateCameraPositionEvent(const EMovementDirection moveDirection) : GEvent(EVENT_TYPE_VULKAN_CAMERA_POSITION_UPDATE), moveDirection(moveDirection) {
}

GUpdateCameraPositionEvent::~GUpdateCameraPositionEvent() {
    
}

const std::string GUpdateCameraPositionEvent::about() {
    std::string aboutString = "GUpdateCameraPositionEvent event";

    switch (moveDirection) {
        case MOVEMENT_DIRECTION_UP:
            aboutString += " moving up";
            break;
        case MOVEMENT_DIRECTION_DOWN:
            aboutString += " moving down";
            break;
        case MOVEMENT_DIRECTION_STRAFE_LEFT:
            aboutString += " moving strafe left";
            break;
        case MOVEMENT_DIRECTION_STRAFE_RIGHT:
            aboutString += " moving strafe right";
            break;
    }

    return aboutString;
}

}
