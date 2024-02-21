#include "glayer.h"

namespace spcGaneshaEngine {

GLayer::GLayer(const std::string& name, GGaneshaContent& content, GEventsServiceProtocol *eventsService) : name(name), content(content), eventsService(eventsService) {
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

std::string GLayer::getName() {
    return name;
}

};
