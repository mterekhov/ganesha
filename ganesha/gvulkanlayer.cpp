#include "gvulkanlayer.h"
#include "gvulkanapi.h"
#include "gupdateviewportevent.h"
#include "gupdateviewmatrixevent.h"

namespace spcGaneshaEngine {

GVULKANLayer::GVULKANLayer(void *metalLayer, GGaneshaContent& content, GEventsServiceProtocol *eventsService) : GLayer("VULKAN", content, eventsService), metalLayer(metalLayer) {
    vulkanAPI = new GVULKANAPI();
}

GVULKANLayer::~GVULKANLayer() {
    delete vulkanAPI;
}

void GVULKANLayer::onAttach() {
    vulkanAPI->initAPI(metalLayer, content);
    vulkanAPI->installIsometricProjection(content.viewport);
    vulkanAPI->installViewMatrix(camera.viewMatrix(content.cameraData.orientation, 
                                                   content.cameraData.positionPoint));
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
            processUpdateViewMatrix(shell.event);
            eventsService->markAsHandled(shell);
            break;
        case EVENT_TYPE_VULKAN_UPDATE_VIEWPORT:
            processUpdateViewport(shell.event);
            eventsService->markAsHandled(shell);
            break;
            
        default:
            break;
    }
    
    return additionalEvents;
}

//  mouse and keyboard events generate this event
std::vector<GEventShell> GVULKANLayer::processUpdateViewMatrix(GEvent *event) {
    GUpdateViewMatrixEvent *updateProjectionEvent = static_cast<GUpdateViewMatrixEvent *>(event);
    vulkanAPI->installViewMatrix(updateProjectionEvent->matrix);

    return { };
}

//  window size changes generate this event
std::vector<GEventShell> GVULKANLayer::processUpdateViewport(GEvent *event) {
    GUpdateViewportEvent *updateViewportEvent = static_cast<GUpdateViewportEvent *>(event);
    content.viewport = updateViewportEvent->viewport;
    vulkanAPI->updateSwapChain(content.viewport);
    vulkanAPI->installIsometricProjection(content.viewport);

    return { };
}

};
