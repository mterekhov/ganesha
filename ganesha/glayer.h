#ifndef SPCGANESHAENGINE_GLAYER_H
#define SPCGANESHAENGINE_GLAYER_H

#include <vector>
#include <string>

#include "geventshell.h"
#include "geventsservice.h"

namespace spcGaneshaEngine {

/// This is the base class for layers in Ganesha engine. Every layer can modify content which will be rendered
class GLayer {
public:
    GLayer(const std::string& name, std::shared_ptr<GEventsServiceProtocol> eventsService);
    virtual ~GLayer();
    
    /// after layer added into processing loop
    virtual void onAttach();
    
    /// After layer removed from processing loop
    virtual void onDetach();
    
    /// Every time when process loop called this one will be called too
    virtual void onUpdate();

    virtual std::vector<GEventShell> onEvent(GEventShell& shell);
    std::string getName();

protected:
    const std::string name;
    std::shared_ptr<GEventsServiceProtocol> eventsService;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GLAYER_H
