#ifndef SPCGANESHAENGINE_GLOAD_GUNDLE_EVENT_H
#define SPCGANESHAENGINE_GLOAD_GUNDLE_EVENT_H

#include <string>

#include "gevent.h"

namespace spcGaneshaEngine {

class GLoadGundleEvent: public GEvent {
public:
    GLoadGundleEvent(const std::string& gundleFilePath);
    virtual ~GLoadGundleEvent();
    
    const std::string gundleFilePath;
    
    const std::string about() override;
};

};

#endif  //  SPCGANESHAENGINE_GLOAD_GUNDLE_EVENT_H
