#include "gapplication.h"

namespace spcGaneshaEngine {

GApplication::GApplication(GEventsServiceProtocol *eventsService, GLayersServiceProtocol *layerService, void *metalLayer, const GGaneshaContent& loadContent) : layerService(layerService), eventsService(eventsService), content(loadContent) {
}

GApplication::~GApplication() {
    layerService->destroy();
    eventsService->destroy();
    
    delete layerService;
    delete eventsService;
}

void GApplication::handleEvent(GEventShell& shell) {
    //  In some cases it is needed that one type of events created another sequenmce of events.
    //  E.g. when position of object changed we need to update data in GPU memory. Or in case we moved camera we need to update projection matrix
    std::vector<GEventShell> additionalEvents = { shell };
    do {
        std::vector<GEventShell> eventsArray = additionalEvents;
        additionalEvents.clear();
        for (auto shellIt = eventsArray.begin(); shellIt != eventsArray.end(); shellIt++) {
            for (auto it = layerService->begin(); it != layerService->end(); it++) {
                GLayer *layer = *it;
                std::vector<GEventShell> tmpEvents = layer->onEvent(*shellIt);
                if (!tmpEvents.empty()) {
                    additionalEvents.insert(additionalEvents.end(), tmpEvents.begin(), tmpEvents.end());
                }
                if (eventsService->doesHandled(*shellIt)) {
                    break;
                }
            }
        }
    } while(!additionalEvents.empty());
}

void GApplication::processRunLoop() {
    for (auto it = layerService->begin(); it != layerService->end(); it++) {
        (*it)->onUpdate();
    }
}

void GApplication::pushLayer(GLayer *layer) {
    layerService->pushLayer(layer);
}

void GApplication::pushOverlay(GLayer *layer) {
    layerService->pushOverlay(layer);
}

};
