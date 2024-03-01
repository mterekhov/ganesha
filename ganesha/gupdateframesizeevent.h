#ifndef SPCGANESHAENGINE_GUPDATEFRAMESIZEEVENT_H
#define SPCGANESHAENGINE_GUPDATEFRAMESIZEEVENT_H

#include "gevent.h"
#include "gviewport.h"

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

#endif  //  SPCGANESHAENGINE_GUPDATEFRAMESIZEEVENT_H
