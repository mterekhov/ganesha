#include "glayer.h"

namespace spcGaneshaEngine {

GLayer::GLayer(GGaneshaContent& content, GEventsServiceProtocol *eventsService) : content(content), eventsService(eventsService) {
}

GLayer::~GLayer() {
}

void GLayer::onAttach() {
}

void GLayer::onDetach() {
}

void GLayer::onUpdate() {
}

std::vector<GEventShell> GLayer::onEvent(GEventShell& shell) {
    return std::vector<GEventShell>();
}

};
