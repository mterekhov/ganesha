#include "geventsservice.h"
#include "gmouseevent.h"
#include "gkeyboardevent.h"
#include "gwindowevent.h"
#include "gupdateviewportevent.h"
#include "gupdateprojectionevent.h"

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
    delete shell.event;
    shell.event = 0;
}

TBool GEventsService::doesHandled(GEventShell& shell) {
    if (shell.event == 0) {
        return true;
    }
    
    return false;
}

GEventShell GEventsService::mouseEvent(const TFloat position_x, const TFloat position_y) {
    GMouseEvent *newEvent = new GMouseEvent(position_x, position_y);
    return GEventShell(newEvent);
}

GEventShell GEventsService::keyboardEvent(const TUInt keyCode) {
    GKeyboardEvent *newEvent = new GKeyboardEvent(keyCode);
    return GEventShell(newEvent);
}

GEventShell GEventsService::windowEvent(const TFloat width, const TFloat height) {
    GWindowEvent *newEvent = new GWindowEvent(width, height);
    return GEventShell(newEvent);
}

GEventShell GEventsService::updateProjectionEvent(const GMatrix& matrix) {
    GUpdateProjectionEvent *newEvent = new GUpdateProjectionEvent(matrix);
    return GEventShell(newEvent);
}

GEventShell GEventsService::updateViewportEvent(const GViewport& viewport) {
    GUpdateViewportEvent *newEvent = new GUpdateViewportEvent(viewport);
    return GEventShell(newEvent);
}

}   //  spcGaneshaEngine
