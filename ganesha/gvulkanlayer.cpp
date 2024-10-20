#include "gvulkanlayer.h"
#include "gvulkanapi.h"
#include "gupdateviewportevent.h"
#include "gupdateviewmatrixevent.h"
#include "gloadgundleevent.h"
#include "gupdatecameralookevent.h"
#include "gupdatecamerapositionevent.h"

namespace spcGaneshaEngine {

GVULKANLayer::GVULKANLayer(const std::string& applicationTitle, void *metalLayer, GEventsServiceProtocol *eventsService) : GLayer("VULKAN", eventsService), vulkanAPI(std::make_shared<GVULKANAPI>(applicationTitle, metalLayer)) {
}

GVULKANLayer::~GVULKANLayer() {
}

void GVULKANLayer::onAttach() {
    vulkanAPI->launchRender(content);
    vulkanAPI->installIsometricProjection(content.viewport);
    vulkanAPI->installViewMatrix(camera.viewMatrix());
}

void GVULKANLayer::onDetach() {
    vulkanAPI->destroyAPI();
}

void GVULKANLayer::onUpdate() {
    vulkanAPI->render();
}

std::vector<GEventShell> GVULKANLayer::onEvent(GEventShell& shell) {
    std::vector<GEventShell> additionalEvents;
    
    switch (shell.event->getType()) {
        case EVENT_TYPE_VULKAN_UPDATE_VIEW_MATRIX:
            additionalEvents = processUpdateViewMatrix(shell.event);
            eventsService->markAsHandled(shell);
            break;
        case EVENT_TYPE_VULKAN_UPDATE_VIEWPORT:
            additionalEvents = processUpdateViewport(shell.event);
            eventsService->markAsHandled(shell);
            break;
        case EVENT_TYPE_VULKAN_LOAD_GUNDLE:
            additionalEvents = processLoadGundle(shell.event);
            eventsService->markAsHandled(shell);
            break;
        case EVENT_TYPE_VULKAN_CAMERA_LOOK_UPDATE:
            additionalEvents = processCameraLookUpdate(shell.event);
            eventsService->markAsHandled(shell);
            break;
        case EVENT_TYPE_VULKAN_CAMERA_POSITION_UPDATE:
            additionalEvents = processCameraPositionUpdate(shell.event);
            eventsService->markAsHandled(shell);
            
        default:
            break;
    }
    
    return additionalEvents;
}

std::vector<GEventShell> GVULKANLayer::processCameraPositionUpdate(std::shared_ptr<GEvent> event) {
    std::shared_ptr<GUpdateCameraPositionEvent> updateCameraPositionEvent = static_pointer_cast<GUpdateCameraPositionEvent>(event);
    
    switch (updateCameraPositionEvent->moveDirection) {
        case MOVEMENT_DIRECTION_UP:
            scene.camera.upCamera();
            break;
        case MOVEMENT_DIRECTION_DOWN:
            scene.camera.downCamera();
            break;
        case MOVEMENT_DIRECTION_FORWARD:  {
            scene.camera.forwardCamera();
        }
            break;
        case MOVEMENT_DIRECTION_BACKWARD: {
            scene.camera.backwardCamera();
        }
            break;
        case MOVEMENT_DIRECTION_STRAFE_LEFT: {
            scene.camera.strafeLeftCamera();
        }
            break;
        case MOVEMENT_DIRECTION_STRAFE_RIGHT: {
            scene.camera.strafeRightCamera();
        }
            break;
        default:
            break;
    }
    
    vulkanAPI->installViewMatrix(scene.camera.viewMatrix());
    
    return { };
}
std::vector<GEventShell> GVULKANLayer::processCameraLookUpdate(std::shared_ptr<GEvent> event) {
    std::shared_ptr<GUpdateCameraLookEvent> updateCameraLookEvent = static_pointer_cast<GUpdateCameraLookEvent>(event);

    scene.camera.mouseCamera(updateCameraLookEvent->lookUpdate.x, updateCameraLookEvent->lookUpdate.y);
    vulkanAPI->installViewMatrix(scene.camera.viewMatrix());

    return { };
}

//  mouse and keyboard events generate this event
std::vector<GEventShell> GVULKANLayer::processUpdateViewMatrix(std::shared_ptr<GEvent> event) {
    std::shared_ptr<GUpdateViewMatrixEvent> updateProjectionEvent = static_pointer_cast<GUpdateViewMatrixEvent>(event);
    vulkanAPI->installViewMatrix(updateProjectionEvent->matrix);

    return { };
}

std::vector<GEventShell> GVULKANLayer::processLoadGundle(std::shared_ptr<GEvent> event) {
    std::shared_ptr<GLoadGundleEvent> loadGundleEvent = static_pointer_cast<GLoadGundleEvent>(event);
    
    vulkanAPI->loadGundle(loadGundleEvent->gundleFilePath);

    return { };
}

//  window size changes generate this event
std::vector<GEventShell> GVULKANLayer::processUpdateViewport(std::shared_ptr<GEvent> event) {
    std::shared_ptr<GUpdateViewportEvent> updateViewportEvent = static_pointer_cast<GUpdateViewportEvent>(event);
    vulkanAPI->updateSwapChain(updateViewportEvent->viewport);
    vulkanAPI->installIsometricProjection(updateViewportEvent->viewport);

    return { };
}

};
