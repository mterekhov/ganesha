#ifndef SPCGANESHAENGINE_GUPDATEVIEWPORTEVENT_H
#define SPCGANESHAENGINE_GUPDATEVIEWPORTEVENT_H

#include "gevent.h"
#include "gviewport.h"

namespace spcGaneshaEngine {

class GUpdateViewportEvent: public GEvent {
public:
    GUpdateViewportEvent(const GViewport& viewport);
    virtual ~GUpdateViewportEvent();
    
    const GViewport viewport;
    
    const std::string about() override;
};

};

#endif  //  SPCGANESHAENGINE_GUPDATEVIEWPORTEVENT_H
