#ifndef SPCGANESHAENGINE_GMOUSEEVENT_H
#define SPCGANESHAENGINE_GMOUSEEVENT_H

#include <string>

#include "gevent.h"
#include "ganeshatypes.h"

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
