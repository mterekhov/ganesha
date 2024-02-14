#ifndef SPCGANESHAENGINE_GEVENTSSERVICE_H
#define SPCGANESHAENGINE_GEVENTSSERVICE_H

#include "geventshell.h"
#include "gmatrix.h"
#include "gviewport.h"

namespace spcGaneshaEngine {

class GEventsServiceProtocol {
public:
    virtual void init() = 0;
    virtual void destroy() = 0;
  
    virtual void markAsHandled(GEventShell& shell) = 0;
    virtual TBool doesHandled(GEventShell& shell) = 0;
    virtual GEventShell mouseEvent(const TFloat position_x, const TFloat position_y) = 0;
    virtual GEventShell keyboardEvent(const TUInt keyCode) = 0;
    virtual GEventShell windowEvent(const TFloat width, const TFloat height) = 0;
    virtual GEventShell updateProjectionEvent(const GMatrix& matrix) = 0;
    virtual GEventShell updateViewportEvent(const GViewport& viewport) = 0;
};

class GEventsService: public GEventsServiceProtocol {
public:
    GEventsService();
    virtual ~GEventsService();
    
    virtual void init();
    virtual void destroy();
    
    virtual void markAsHandled(GEventShell& shell);
    virtual TBool doesHandled(GEventShell& shell);
    virtual GEventShell mouseEvent(const TFloat position_x, const TFloat position_y);
    virtual GEventShell keyboardEvent(const TUInt keyCode);
    virtual GEventShell windowEvent(const TFloat width, const TFloat height);
    virtual GEventShell updateProjectionEvent(const GMatrix& matrix);
    virtual GEventShell updateViewportEvent(const GViewport& viewport);
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GEVENTSSERVICE_H
