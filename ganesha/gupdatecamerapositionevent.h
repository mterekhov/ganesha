#ifndef SPCGANESHAENGINE_GUPDATE_CAMERA_POSITION_EVENT_H
#define SPCGANESHAENGINE_GUPDATE_CAMERA_POSITION_EVENT_H

#include <string>

#include "gevent.h"
#include "gcameradata.h"

namespace spcGaneshaEngine {

class GUpdateCameraPositionEvent: public GEvent {
public:
    GUpdateCameraPositionEvent(const EMovementDirection moveDirection);
    virtual ~GUpdateCameraPositionEvent();
    
    const EMovementDirection moveDirection;
    
    const std::string about() override;
};

};

#endif  //  SPCGANESHAENGINE_GUPDATE_CAMERA_POSITION_EVENT_H
