#include "gsystemlayer.h"
#include "gkeyboardevent.h"
#include "gwindowevent.h"
#include "gmouseevent.h"
#include "gupdateprojectionevent.h"
#include "gupdateviewportevent.h"

namespace spcGaneshaEngine {

GSystemLayer::GSystemLayer(const std::string& jsonKeyBindings, GGaneshaContent& content, GEventsServiceProtocol *eventsService) : GLayer(content, eventsService) {
}

GSystemLayer::~GSystemLayer() {
}

std::vector<GEventShell> GSystemLayer::onEvent(GEventShell& shell) {
    std::vector<GEventShell> additionalEvents;
    
    switch (shell.event->getType()) {
        case EVENT_TYPE_KEY_DOWN:
            additionalEvents = processKeyboard(shell.event);
            break;
        case EVENT_TYPE_WINDOW_RESIZE:
            additionalEvents = processWindowResize(shell.event);
            break;
        case EVENT_TYPE_MOUSE_MOVED:
            additionalEvents = processMouseMove(shell.event);
            break;
            
        default:
            break;
    }
    
    eventsService->markAsHandled(shell);
    return additionalEvents;
}

std::vector<GEventShell> GSystemLayer::processMouseMove(GEvent *event) {
    GMouseEvent *mouseEvent = static_cast<GMouseEvent *>(event);

    content.cameraData.orientation = camera.mouseCamera(content.cameraData.orientation,
                                                        mouseEvent->position_x,
                                                        mouseEvent->position_y,
                                                        content.cameraData.mouseSens);
    
    GMatrix newCameraMatrix = camera.viewMatrix(content.cameraData.orientation, content.cameraData.positionPoint);
    return { eventsService->updateProjectionEvent(newCameraMatrix) };
}

std::vector<GEventShell> GSystemLayer::processWindowResize(GEvent *event) {
    GWindowEvent *windowEvent = static_cast<GWindowEvent *>(event);

    content.viewport.width = windowEvent->width;
    content.viewport.height = windowEvent->height;
    
    return { eventsService->updateViewportEvent(content.viewport) };
}

std::vector<GEventShell> GSystemLayer::processKeyboard(GEvent *event) {
    GKeyboardEvent *keyboardEvent = static_cast<GKeyboardEvent *>(event);

    switch (keyboardEvent->keyCode) {
        case 14:
            content.cameraData.positionPoint = camera.downCamera(content.cameraData.positionPoint, content.cameraData.keyboardSpeed);
            break;
        case 12:
            content.cameraData.positionPoint = camera.upCamera(content.cameraData.positionPoint, content.cameraData.keyboardSpeed);
            break;
        case 13:
            content.cameraData.positionPoint = camera.forwardCamera(content.cameraData.positionPoint, content.cameraData.keyboardSpeed);
            break;
        case 1:
            content.cameraData.positionPoint = camera.backwardCamera(content.cameraData.positionPoint, content.cameraData.keyboardSpeed);
            break;
        case 0:
            content.cameraData.positionPoint = camera.strafeLeftCamera(content.cameraData.positionPoint, content.cameraData.keyboardSpeed);
            break;
        case 2:
            content.cameraData.positionPoint = camera.strafeRightCamera(content.cameraData.positionPoint, content.cameraData.keyboardSpeed);
            break;
        default:
            break;
    }
    
    GMatrix newCameraMatrix = camera.viewMatrix(content.cameraData.orientation, content.cameraData.positionPoint);
    return { eventsService->updateProjectionEvent(newCameraMatrix) };
}

};
