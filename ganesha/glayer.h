#ifndef SPCGANESHAENGINE_GLAYER_H
#define SPCGANESHAENGINE_GLAYER_H

#include <vector>

#include "geventshell.h"
#include "gscene.h"
#include "geventsservice.h"

namespace spcGaneshaEngine {

/// This is the base class for layers in Ganesha engine. Every layer can modify content which will be rendered
class GLayer {
public:
    GLayer(const std::string& name, GScene& content, GEventsServiceProtocol *eventsService);
    virtual ~GLayer();

    virtual void onAttach();
    virtual void onDetach();
    virtual void onUpdate();
    virtual std::vector<GEventShell> onEvent(GEventShell& shell);
    std::string getName();

protected:
    const std::string name;
    GScene& content;
    GEventsServiceProtocol *eventsService;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GLAYER_H
