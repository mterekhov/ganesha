#include "gupdateviewmatrixevent.h"

namespace spcGaneshaEngine {

GUpdateViewMatrixEvent::GUpdateViewMatrixEvent(const GMatrix& matrix) : GEvent(EVENT_TYPE_VULKAN_UPDATE_VIEW_MATRIX), matrix(matrix) {
    
}

GUpdateViewMatrixEvent::~GUpdateViewMatrixEvent() {
    
}

}
