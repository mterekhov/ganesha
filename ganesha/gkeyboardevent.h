#ifndef SPCGANESHAENGINE_GKEYBOARDEVENT_H
#define SPCGANESHAENGINE_GKEYBOARDEVENT_H

#include "gevent.h"

namespace spcGaneshaEngine {

class GKeyboardEvent: public GEvent {
public:
    GKeyboardEvent(const TUInt keyCode);
    virtual ~GKeyboardEvent();
    
    const TUInt keyCode;
    
    const std::string about() override;
};

};

#endif  //  SPCGANESHAENGINE_GKEYBOARDEVENT_H
