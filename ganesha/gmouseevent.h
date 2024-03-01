#ifndef SPCGANESHAENGINE_GMOUSEEVENT_H
#define SPCGANESHAENGINE_GMOUSEEVENT_H

#include "gevent.h"

namespace spcGaneshaEngine {

class GMouseEvent: public GEvent {
public:
    GMouseEvent(const TFloat position_x, const TFloat position_y);
    virtual ~GMouseEvent();

    const TFloat position_x;
    const TFloat position_y;
    const std::string about() override;
};

};

#endif  //  SPCGANESHAENGINE_GMOUSEEVENT_H
