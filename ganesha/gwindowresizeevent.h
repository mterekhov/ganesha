#ifndef SPCGANESHAENGINE_GWINDOW_RESIZE_EVENT_H
#define SPCGANESHAENGINE_GWINDOW_RESIZE_EVENT_H

#include "gevent.h"

namespace spcGaneshaEngine {

class GWindowResizeEvent: public GEvent {
public:
    GWindowResizeEvent(const TFloat width, const TFloat height);
    virtual ~GWindowResizeEvent();

    const TFloat width;
    const TFloat height;
    const std::string about() override;
};

};

#endif  //  SPCGANESHAENGINE_GWINDOW_RESIZE_EVENT_H
