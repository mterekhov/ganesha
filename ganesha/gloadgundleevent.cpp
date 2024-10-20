#include "gloadgundleevent.h"

namespace spcGaneshaEngine {

GLoadGundleEvent::GLoadGundleEvent(const std::string& gundleFilePath) : GEvent(EVENT_TYPE_VULKAN_LOAD_GUNDLE), gundleFilePath(gundleFilePath) {
    
}

GLoadGundleEvent::~GLoadGundleEvent() {
    
}

const std::string GLoadGundleEvent::about() {
    std::string aboutString = "GLoadContentEvent loading GUNDLE <";

    aboutString += gundleFilePath;
    aboutString += ">";
    
    return aboutString;
}

}
