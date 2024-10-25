#include "geventsservice.h"
#include "gmouseevent.h"
#include "gkeyboardevent.h"
#include "gupdateframesizeevent.h"
#include "gloadgundleevent.h"
#include "gupdatecameralookevent.h"
#include "gupdatecamerapositionevent.h"
#include "gwindowresizeevent.h"

namespace spcGaneshaEngine {

GEventsService::GEventsService() {
    
}

GEventsService::~GEventsService() {
    
}

void GEventsService::init() {
    
}

void GEventsService::destroy() {
}

void GEventsService::markAsHandled(GEventShell& shell) {
   shell.event = nullptr;
}

TBool GEventsService::doesHandled(GEventShell& shell) {
    if (shell.event == nullptr) {
        return true;
    }
    
    return false;
}

GEventShell GEventsService::windowResizeEvent(const TFloat width, const TFloat height) {
    std::shared_ptr<GWindowResizeEvent> newEvent = std::make_shared<GWindowResizeEvent>(width, height);
    return GEventShell(newEvent);

}

GEventShell GEventsService::mouseEvent(const TFloat position_x, const TFloat position_y) {
    std::shared_ptr<GMouseEvent> newEvent = std::make_shared<GMouseEvent>(position_x, position_y);
    return GEventShell(newEvent);
}

GEventShell GEventsService::keyboardEvent(const TUInt keyCode) {
    std::shared_ptr<GKeyboardEvent> newEvent = std::make_shared<GKeyboardEvent>(keyCode);
    return GEventShell(newEvent);
}

GEventShell GEventsService::updateFrameSizeEvent(const TFloat width, const TFloat height) {
    std::shared_ptr<GUpdateFrameSizeEvent> newEvent = std::make_shared<GUpdateFrameSizeEvent>(width, height);
    return GEventShell(newEvent);

}

GEventShell GEventsService::loadGundle(const std::string& gundleFilePath) {
    std::shared_ptr<GLoadGundleEvent> newEvent = std::make_shared<GLoadGundleEvent>(gundleFilePath);
    return GEventShell(newEvent);
}

GEventShell GEventsService::cameraLookUpdate(const GPoint2D& lookUpdate) {
    std::shared_ptr<GUpdateCameraLookEvent> newEvent = std::make_shared<GUpdateCameraLookEvent>(lookUpdate);
    return GEventShell(newEvent);;
}

GEventShell GEventsService::cameraPositionUpdate(const EMovementDirection moveDirection) {
    std::shared_ptr<GUpdateCameraPositionEvent> newEvent = std::make_shared<GUpdateCameraPositionEvent>(moveDirection);
    return GEventShell(newEvent);;
}

}   //  spcGaneshaEngine
