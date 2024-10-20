#ifndef SPCGANESHAENGINE_GEVENTSHELL_H
#define SPCGANESHAENGINE_GEVENTSHELL_H

#include "gevent.h"

namespace spcGaneshaEngine {

class GEventShell {
public:
    GEventShell(std::shared_ptr<GEvent> event);
    ~GEventShell();
    
    std::shared_ptr<GEvent> event;
};

};

#endif  //  SPCGANESHAENGINE_GEVENTSHELL_H
