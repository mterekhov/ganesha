#ifndef SPCGANESHAENGINE_GVULKANLAYER_H
#define SPCGANESHAENGINE_GVULKANLAYER_H

#include "glayer.h"

namespace spcGaneshaEngine {

/// This layer should process keyboard, mouse and window events
class GVULKANLayer : public GLayer {
public:
    GVULKANLayer(GGaneshaContent& content, GEventsServiceProtocol *eventsService);
    virtual ~GVULKANLayer();

    virtual std::vector<GEventShell> onEvent(GEventShell& shell);
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GVULKANLAYER_H
