#ifndef SPCGANESHAENGINE_GSYSTEMLAYER_H
#define SPCGANESHAENGINE_GSYSTEMLAYER_H

#include "glayer.h"
#include "gcamera.h"

namespace spcGaneshaEngine {

/// This layer should process keyboard, mouse and window events
class GSystemLayer : public GLayer {
public:
    GSystemLayer(const std::string& jsonKeyBindings, GScene& content, GEventsServiceProtocol *eventsService);
    virtual ~GSystemLayer();

    std::vector<GEventShell> onEvent(GEventShell& shell) override;
    
private:
    std::vector<GEventShell> processKeyboard(GEvent *event);
    std::vector<GEventShell> processWindowResize(GEvent *event);
    std::vector<GEventShell> processMouseMove(GEvent *event);

    std::vector<GEventShell> cameraPositionEvent(const GPoint& eyePoint, const GPoint& targetPoint, const GVector& upVector);

    GCamera camera;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GSYSTEMLAYER_H
