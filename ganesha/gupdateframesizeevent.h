#ifndef SPCGANESHAENGINE_GUPDATE_FRAMESIZE_EVENT_H
#define SPCGANESHAENGINE_GUPDATE_FRAMESIZE_EVENT_H

#include <string>

#include "gevent.h"
#include "ganeshatypes.h"

namespace spcGaneshaEngine {

class GUpdateFrameSizeEvent: public GEvent {
public:
    GUpdateFrameSizeEvent(const TFloat width, const TFloat height);
    virtual ~GUpdateFrameSizeEvent();
    
    const TFloat width;
    const TFloat height;

    const std::string about() override;
};

};

#endif  //  SPCGANESHAENGINE_GUPDATE_FRAMESIZE_EVENT_H
