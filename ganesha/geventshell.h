#ifndef SPCGANESHAENGINE_GEVENTSHELL_H
#define SPCGANESHAENGINE_GEVENTSHELL_H

#include "gevent.h"

namespace spcGaneshaEngine {

class GEventShell {
public:
    GEventShell(GEvent *event);
    ~GEventShell();
    
    GEvent *event;
};

};

#endif  //  SPCGANESHAENGINE_GEVENTSHELL_H
