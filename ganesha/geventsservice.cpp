#include "geventsservice.h"
#include "gmouseevent.h"
#include "gkeyboardevent.h"
#include "gupdateviewportevent.h"
#include "gupdateviewmatrixevent.h"
#include "gloadgundleevent.h"
#include "gupdatecameralookevent.h"
#include "gupdatecamerapositionevent.h"

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

GEventShell GEventsService::mouseEvent(const TFloat position_x, const TFloat position_y) {
    std::shared_ptr<GMouseEvent> newEvent = std::make_shared<GMouseEvent>(position_x, position_y);
    return GEventShell(newEvent);
}

GEventShell GEventsService::keyboardEvent(const TUInt keyCode) {
    std::shared_ptr<GKeyboardEvent> newEvent = std::make_shared<GKeyboardEvent>(keyCode);
    return GEventShell(newEvent);
}

GEventShell GEventsService::updateViewMatrixEvent(const GMatrix& matrix) {
    std::shared_ptr<GUpdateViewMatrixEvent> newEvent = std::make_shared<GUpdateViewMatrixEvent>(matrix);
    return GEventShell(newEvent);
}

GEventShell GEventsService::updateViewportEvent(const GViewport& viewport) {
    std::shared_ptr<GUpdateViewportEvent> newEvent = std::make_shared<GUpdateViewportEvent>(viewport);
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
