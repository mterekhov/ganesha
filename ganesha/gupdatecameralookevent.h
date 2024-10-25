#ifndef SPCGANESHAENGINE_GUPDATE_CAMERA_LOOK_EVENT_H
#define SPCGANESHAENGINE_GUPDATE_CAMERA_LOOK_EVENT_H

#include <string>

#include "gevent.h"
#include "gpoint2d.h"

namespace spcGaneshaEngine {

class GUpdateCameraLookEvent: public GEvent {
public:
    GUpdateCameraLookEvent(const GPoint2D& lookUpdate);
    virtual ~GUpdateCameraLookEvent();
    
    const GPoint2D lookUpdate;
    
    const std::string about() override;
};

};

#endif  //  SPCGANESHAENGINE_GUPDATE_CAMERA_LOOK_EVENT_H
