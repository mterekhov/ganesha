#include "gsystemlayer.h"
#include "gkeyboardevent.h"
#include "gwindowevent.h"
#include "gmouseevent.h"
#include "gupdateviewmatrixevent.h"
#include "gupdateframesizeevent.h"
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
    
    return { eventsService->updateFrameSizeEvent(windowEvent->width, windowEvent->height) };
}

std::vector<GEventShell> GSystemLayer::processMouseMove(GEvent *event) {
    GMouseEvent *mouseEvent = static_cast<GMouseEvent *>(event);

    content.cameraData.orientation = camera.mouseCamera(content.cameraData.orientation,
                                                        mouseEvent->position_x,
                                                        mouseEvent->position_y,
                                                        content.cameraData.mouseSens);
    
    GMatrix newCameraMatrix = camera.viewMatrix(content.cameraData.orientation, content.cameraData.positionPoint);
    return { eventsService->updateViewMatrixEvent(newCameraMatrix) };
}

std::vector<GEventShell> GSystemLayer::processKeyboard(GEvent *event) {
    GKeyboardEvent *keyboardEvent = static_cast<GKeyboardEvent *>(event);
    std::vector<GEventShell> newEvents = { };

    switch (keyboardEvent->keyCode) {
        case MACKEYCODE_E:
            content.cameraData.positionPoint = camera.downCamera(content.cameraData.positionPoint, content.cameraData.keyboardSpeed);
            newEvents = cameraPositionEvent(content.cameraData.orientation, content.cameraData.positionPoint);
            break;
        case MACKEYCODE_Q:
            content.cameraData.positionPoint = camera.upCamera(content.cameraData.positionPoint, content.cameraData.keyboardSpeed);
            newEvents = cameraPositionEvent(content.cameraData.orientation, content.cameraData.positionPoint);
            break;
        case MACKEYCODE_W:
            content.cameraData.positionPoint = camera.forwardCamera(content.cameraData.positionPoint, content.cameraData.keyboardSpeed);
            newEvents = cameraPositionEvent(content.cameraData.orientation, content.cameraData.positionPoint);
            break;
        case MACKEYCODE_S:
            content.cameraData.positionPoint = camera.backwardCamera(content.cameraData.positionPoint, content.cameraData.keyboardSpeed);
            newEvents = cameraPositionEvent(content.cameraData.orientation, content.cameraData.positionPoint);
            break;
        case MACKEYCODE_A: {
            GLOG_INFO("old position %.3f\t%.3f\t%.3f\n", 
                      content.cameraData.positionPoint.x,
                      content.cameraData.positionPoint.y,
                      content.cameraData.positionPoint.z);
            content.cameraData.positionPoint = camera.strafeLeftCamera(content.cameraData.positionPoint, 0.01);//content.cameraData.keyboardSpeed);
            GLOG_INFO("new position %.3f\t%.3f\t%.3f\n", 
                      content.cameraData.positionPoint.x,
                      content.cameraData.positionPoint.y,
                      content.cameraData.positionPoint.z);
            newEvents = cameraPositionEvent(content.cameraData.orientation, content.cameraData.positionPoint);
        }
        break;
        case MACKEYCODE_D:
            content.cameraData.positionPoint = camera.strafeRightCamera(content.cameraData.positionPoint, content.cameraData.keyboardSpeed);
            break;
        default:
            break;
    }
    
    return newEvents;
}

std::vector<GEventShell> GSystemLayer::cameraPositionEvent(const GQuaternion& orientation, const GPoint& positionPoint) {
    GMatrix newCameraMatrix = camera.viewMatrix(orientation, positionPoint);
    GLOG_INFO("matrix:\n%s\n\n\n", newCameraMatrix.print().c_str());

    return { eventsService->updateViewMatrixEvent(newCameraMatrix) };
}

};
