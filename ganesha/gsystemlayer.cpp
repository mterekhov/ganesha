#include "gsystemlayer.h"
#include "gkeyboardevent.h"
#include "gwindowevent.h"
#include "gmouseevent.h"
#include "gupdateviewmatrixevent.h"
#include "gupdateviewportevent.h"
#include "mackeycode.h"
#include "glog.h"

namespace spcGaneshaEngine {

GSystemLayer::GSystemLayer(const std::string& jsonKeyBindings, GGaneshaContent& content, GEventsServiceProtocol *eventsService) : GLayer("system", content, eventsService) {
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

    content.cameraData.targetPoint = camera.mouseCamera(content.cameraData.targetPoint,
                                                        mouseEvent->position_x,
                                                        mouseEvent->position_y,
                                                        content.cameraData.mouseSens);

    GMatrix newCameraMatrix = camera.viewMatrix(content.cameraData.eyePoint,
                                                content.cameraData.targetPoint,
                                                content.cameraData.upVector);

    return { eventsService->updateViewMatrixEvent(newCameraMatrix) };
}

std::vector<GEventShell> GSystemLayer::processKeyboard(GEvent *event) {
    GKeyboardEvent *keyboardEvent = static_cast<GKeyboardEvent *>(event);
    std::vector<GEventShell> newEvents = { };

    switch (keyboardEvent->keyCode) {
        case MACKEYCODE_E:
            content.cameraData.targetPoint.y += content.cameraData.keyboardSpeed;
            content.cameraData.eyePoint.y += content.cameraData.keyboardSpeed;
            newEvents = cameraPositionEvent(content.cameraData.eyePoint,
                                            content.cameraData.targetPoint,
                                            content.cameraData.upVector);
            break;
        case MACKEYCODE_Q:
            content.cameraData.targetPoint.y -= content.cameraData.keyboardSpeed;
            content.cameraData.eyePoint.y -= content.cameraData.keyboardSpeed;
            newEvents = cameraPositionEvent(content.cameraData.eyePoint,
                                            content.cameraData.targetPoint,
                                            content.cameraData.upVector);
            break;
        case MACKEYCODE_W:
            content.cameraData.targetPoint.x -= content.cameraData.keyboardSpeed;
            content.cameraData.targetPoint.y -= content.cameraData.keyboardSpeed;
            content.cameraData.targetPoint.z -= content.cameraData.keyboardSpeed;

            content.cameraData.eyePoint.x -= content.cameraData.keyboardSpeed;
            content.cameraData.eyePoint.y -= content.cameraData.keyboardSpeed;
            content.cameraData.eyePoint.z -= content.cameraData.keyboardSpeed;
            newEvents = cameraPositionEvent(content.cameraData.eyePoint,
                                            content.cameraData.targetPoint,
                                            content.cameraData.upVector);
            break;
        case MACKEYCODE_S:
            content.cameraData.targetPoint.x += content.cameraData.keyboardSpeed;
            content.cameraData.targetPoint.y += content.cameraData.keyboardSpeed;
            content.cameraData.targetPoint.z += content.cameraData.keyboardSpeed;

            content.cameraData.eyePoint.x += content.cameraData.keyboardSpeed;
            content.cameraData.eyePoint.y += content.cameraData.keyboardSpeed;
            content.cameraData.eyePoint.z += content.cameraData.keyboardSpeed;
            newEvents = cameraPositionEvent(content.cameraData.eyePoint,
                                            content.cameraData.targetPoint,
                                            content.cameraData.upVector);
            break;
//        case MACKEYCODE_A: {
//            content.cameraData.positionPoint = camera.strafeLeftCamera(content.cameraData.positionPoint, content.cameraData.keyboardSpeed);
//            newEvents = cameraPositionEvent(content.cameraData.orientation, content.cameraData.positionPoint);
//        }
//        break;
//        case MACKEYCODE_D:
//            content.cameraData.positionPoint = camera.strafeRightCamera(content.cameraData.positionPoint, content.cameraData.keyboardSpeed);
//            break;
        default:
            break;
    }
    
    return newEvents;
}

std::vector<GEventShell> GSystemLayer::cameraPositionEvent(const GPoint& eyePoint, const GPoint& targetPoint, const GVector& upVector) {
    GMatrix newCameraMatrix = camera.viewMatrix(eyePoint, targetPoint, upVector);
    return { eventsService->updateViewMatrixEvent(newCameraMatrix) };
}

};
