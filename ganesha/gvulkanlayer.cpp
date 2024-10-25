#include "gvulkanlayer.h"
#include "gupdateframesizeevent.h"
#include "gloadgundleevent.h"
#include "gupdatecameralookevent.h"
#include "gupdatecamerapositionevent.h"

namespace spcGaneshaEngine {

GVULKANLayer::GVULKANLayer(const std::string& applicationTitle, void *metalLayer, std::shared_ptr<GEventsServiceProtocol> eventsService) : GLayer("VULKAN", eventsService), vulkanAPI(std::make_shared<GVULKANAPI>(applicationTitle, metalLayer)) {
}

GVULKANLayer::~GVULKANLayer() {
}

void GVULKANLayer::onAttach() {
    vulkanAPI->installIsometricProjection(scene.viewport);
    vulkanAPI->installViewMatrix(scene.camera.viewMatrix());
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
        case EVENT_TYPE_VULKAN_UPDATE_FRAME_SIZE:
            additionalEvents = processUpdateFrameSize(shell.event);
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

std::vector<GEventShell> GVULKANLayer::processLoadGundle(std::shared_ptr<GEvent> event) {
    std::shared_ptr<GLoadGundleEvent> loadGundleEvent = static_pointer_cast<GLoadGundleEvent>(event);
    
    vulkanAPI->loadGundle(loadGundleEvent->gundleFilePath);

    return { };
}

std::vector<GEventShell> GVULKANLayer::processUpdateFrameSize(std::shared_ptr<GEvent> event) {
    std::shared_ptr<GUpdateFrameSizeEvent> updateFrameSizeEvent = static_pointer_cast<GUpdateFrameSizeEvent>(event);

    scene.viewport.width = updateFrameSizeEvent->width;
    scene.viewport.height = updateFrameSizeEvent->height;
    vulkanAPI->updateSwapChain(scene.viewport.width, scene.viewport.height);
    vulkanAPI->installIsometricProjection(scene.viewport);

    return { };
}

};
