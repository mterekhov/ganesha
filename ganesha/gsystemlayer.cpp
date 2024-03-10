#include "gsystemlayer.h"
#include "gkeyboardevent.h"
#include "gwindowevent.h"
#include "gmouseevent.h"
#include "gupdateviewmatrixevent.h"
#include "gupdateviewportevent.h"
#include "mackeycode.h"
#include "glog.h"

namespace spcGaneshaEngine {

GSystemLayer::GSystemLayer(const std::string& jsonKeyBindings, GScene& content, GEventsServiceProtocol *eventsService) : GLayer("system", content, eventsService), camera(content.cameraData) {
}

GSystemLayer::~GSystemLayer() {
}

std::vector<GEventShell> GSystemLayer::onEvent(GEventShell& shell) {
    std::vector<GEventShell> additionalEvents;
    
    switch (shell.event->getType()) {
        case EVENT_TYPE_KEY_DOWN:
            additionalEvents = processKeyboard(shell.event);
            eventsService->markAsHandled(shell);
            break;
        case EVENT_TYPE_WINDOW_RESIZE:
            additionalEvents = processWindowResize(shell.event);
            eventsService->markAsHandled(shell);
            break;
        case EVENT_TYPE_MOUSE_MOVED:
            additionalEvents = processMouseMove(shell.event);
            eventsService->markAsHandled(shell);
            break;
            
        default:
            break;
    }
    
    return additionalEvents;
}

std::vector<GEventShell> GSystemLayer::processWindowResize(GEvent *event) {
    GWindowEvent *windowEvent = static_cast<GWindowEvent *>(event);

    content.viewport.width = windowEvent->width;
    content.viewport.height = windowEvent->height;
    
    return { eventsService->updateViewportEvent(content.viewport) };
}

std::vector<GEventShell> GSystemLayer::processMouseMove(GEvent *event) {
    GMouseEvent *mouseEvent = static_cast<GMouseEvent *>(event);

    camera.mouseCamera(mouseEvent->position_x, mouseEvent->position_y);
    return { eventsService->updateViewMatrixEvent(camera.viewMatrix()) };
}

std::vector<GEventShell> GSystemLayer::processKeyboard(GEvent *event) {
    GKeyboardEvent *keyboardEvent = static_cast<GKeyboardEvent *>(event);
    std::vector<GEventShell> newEvents = { };

    switch (keyboardEvent->keyCode) {
        case MACKEYCODE_E:
            camera.upCamera();
            newEvents = cameraPositionEvent(content.cameraData.eyePoint,
                                            content.cameraData.targetPoint,
                                            content.cameraData.upVector);
            break;
        case MACKEYCODE_Q:
            camera.downCamera();
            newEvents = cameraPositionEvent(content.cameraData.eyePoint,
                                            content.cameraData.targetPoint,
                                            content.cameraData.upVector);
            break;
        case MACKEYCODE_W:  {
            camera.forwardCamera();
            newEvents = cameraPositionEvent(content.cameraData.eyePoint,
                                            content.cameraData.targetPoint,
                                            content.cameraData.upVector);
        }
            break;
        case MACKEYCODE_S: {
            camera.backwardCamera();
            newEvents = cameraPositionEvent(content.cameraData.eyePoint,
                                            content.cameraData.targetPoint,
                                            content.cameraData.upVector);
        }
            break;
        case MACKEYCODE_A: {
            camera.strafeLeftCamera();
            newEvents = cameraPositionEvent(content.cameraData.eyePoint,
                                            content.cameraData.targetPoint,
                                            content.cameraData.upVector);
        }
        break;
        case MACKEYCODE_D: {
            camera.strafeRightCamera();
            newEvents = cameraPositionEvent(content.cameraData.eyePoint,
                                            content.cameraData.targetPoint,
                                            content.cameraData.upVector);
        }
            break;
        default:
            break;
    }
    
    return newEvents;
}

std::vector<GEventShell> GSystemLayer::cameraPositionEvent(const GPoint& eyePoint, const GPoint& targetPoint, const GVector& upVector) {
    GMatrix newCameraMatrix = camera.viewMatrix();
    return { eventsService->updateViewMatrixEvent(newCameraMatrix) };
}

}
