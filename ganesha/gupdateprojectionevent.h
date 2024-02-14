#ifndef SPCGANESHAENGINE_GUPDATEPROJECTIONEVENT_H
#define SPCGANESHAENGINE_GUPDATEPROJECTIONEVENT_H

#include "gevent.h"
#include "gmatrix.h"

namespace spcGaneshaEngine {

class GUpdateProjectionEvent: public GEvent {
public:
    GUpdateProjectionEvent(const GMatrix& matrix);
    virtual ~GUpdateProjectionEvent();
    
    const GMatrix matrix;
};

};

#endif  //  SPCGANESHAENGINE_GUPDATEPROJECTIONEVENT_H
