#include "gvulkanlayer.h"
#include "gvulkanapi.h"
#include "gupdateframesizeevent.h"
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
    vulkanAPI->installIsometricProjection(content.viewport.fov,
                                          content.viewport.near,
                                          content.viewport.far);
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
            break;
        case EVENT_TYPE_VULKAN_UPDATE_FRAME_SIZE:
            processUpdateFrameSize(shell.event);
            break;
            
        default:
            break;
    }
    
    eventsService->markAsHandled(shell);
    return additionalEvents;
}

//  mouse and keyboard events generate this event
std::vector<GEventShell> GVULKANLayer::processUpdateViewMatrix(GEvent *event) {
    GUpdateViewMatrixEvent *updateProjectionEvent = static_cast<GUpdateViewMatrixEvent *>(event);
    vulkanAPI->installViewMatrix(updateProjectionEvent->matrix);

    return { };
}

//  window size changes generate this event
std::vector<GEventShell> GVULKANLayer::processUpdateFrameSize(GEvent *event) {
    GUpdateFrameSizeEvent *updateViewportEvent = static_cast<GUpdateFrameSizeEvent *>(event);
    vulkanAPI->frameResized(updateViewportEvent->width,
                            updateViewportEvent->height);

    return { };
}

};
