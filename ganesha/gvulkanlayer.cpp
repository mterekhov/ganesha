#include "gvulkanlayer.h"

namespace spcGaneshaEngine {

GVULKANLayer::GVULKANLayer(GGaneshaContent& content, GEventsServiceProtocol *eventsService) : GLayer(content, eventsService) {
}

GVULKANLayer::~GVULKANLayer() {
}

std::vector<GEventShell> GVULKANLayer::onEvent(GEventShell& shell) {
    std::vector<GEventShell> additionalEvents;

    switch (shell.event->getType()) {
        case EVENT_TYPE_VULKAN_UPDATE_PROJECTION:
            break;
        case EVENT_TYPE_VULKAN_UPDATE_VIEWPORT:
            break;

        default:
            break;
    }

    eventsService->markAsHandled(shell);
    return additionalEvents;
}

};
