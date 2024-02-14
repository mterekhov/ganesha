#include "gupdateprojectionevent.h"

namespace spcGaneshaEngine {

GUpdateProjectionEvent::GUpdateProjectionEvent(const GMatrix& matrix) : GEvent(EVENT_TYPE_VULKAN_UPDATE_PROJECTION), matrix(matrix) {
    
}

GUpdateProjectionEvent::~GUpdateProjectionEvent() {
    
}

}
