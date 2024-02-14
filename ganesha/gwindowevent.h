#ifndef SPCGANESHAENGINE_GWINDOWEVENT_H
#define SPCGANESHAENGINE_GWINDOWEVENT_H

#include "gevent.h"

namespace spcGaneshaEngine {

class GWindowEvent: public GEvent {
public:
    GWindowEvent(const TFloat width, const TFloat height);
    virtual ~GWindowEvent();

    const TFloat width;
    const TFloat height;
};

};

#endif  //  SPCGANESHAENGINE_GWINDOWEVENT_H
