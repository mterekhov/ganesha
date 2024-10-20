#include "gsystemlayer.h"
#include "gkeyboardevent.h"
#include "gmouseevent.h"
#include "gupdateviewmatrixevent.h"
#include "gupdateviewportevent.h"
#include "mackeycode.h"
#include "glog.h"

namespace spcGaneshaEngine {

GSystemLayer::GSystemLayer(GEventsServiceProtocol *eventsService) : GLayer("system", eventsService) {
}

GSystemLayer::~GSystemLayer() {
}

std::vector<GEventShell> GSystemLayer::onEvent(GEventShell& shell) {
    std::vector<GEventShell> additionalEvents;
    
    switch (shell.event->getType()) {
        case EVENT_TYPE_WINDOW_RESIZED:
            additionalEvents = processWindowResize(shell.event);
            eventsService->markAsHandled(shell);
            break;
        case EVENT_TYPE_MOUSE_MOVED:
            additionalEvents = processMouseMove(shell.event);
            eventsService->markAsHandled(shell);
            break;
        case EVENT_TYPE_KEY_DOWN:
            additionalEvents = processKeyboard(shell.event);
            eventsService->markAsHandled(shell);
            break;
            
        default:
            break;
    }
    
    return additionalEvents;
}

std::vector<GEventShell> GSystemLayer::processMouseMove(std::shared_ptr<GEvent> event) {
    std::shared_ptr<GMouseEvent> mouseEvent = static_pointer_cast<GMouseEvent>(event);
    return { eventsService->cameraLookUpdate({ mouseEvent->position_x, mouseEvent->position_y }) };
}

std::vector<GEventShell> GSystemLayer::processKeyboard(std::shared_ptr<GEvent> event) {
    std::shared_ptr<GKeyboardEvent> keyboardEvent = static_pointer_cast<GKeyboardEvent>(event);
    std::vector<GEventShell> newEvents = { };
    
    switch (keyboardEvent->keyCode) {
        case MACKEYCODE_E:
            newEvents.push_back(eventsService->cameraPositionUpdate(MOVEMENT_DIRECTION_UP));
            break;
        case MACKEYCODE_Q:
            newEvents.push_back(eventsService->cameraPositionUpdate(MOVEMENT_DIRECTION_DOWN));
            break;
        case MACKEYCODE_W:  {
            newEvents.push_back(eventsService->cameraPositionUpdate(MOVEMENT_DIRECTION_FORWARD));
        }
            break;
        case MACKEYCODE_S: {
            newEvents.push_back(eventsService->cameraPositionUpdate(MOVEMENT_DIRECTION_BACKWARD));
        }
            break;
        case MACKEYCODE_A: {
            newEvents.push_back(eventsService->cameraPositionUpdate(MOVEMENT_DIRECTION_STRAFE_LEFT));
        }
            break;
        case MACKEYCODE_D: {
            newEvents.push_back(eventsService->cameraPositionUpdate(MOVEMENT_DIRECTION_STRAFE_RIGHT));
        }
            break;
        default:
            break;
    }
    
    return newEvents;
}

std::vector<GEventShell> GSystemLayer::processWindowResize(std::shared_ptr<GEvent> event) {
    
}

}
