#ifndef SPCGANESHAENGINE_GGRAPHICSAPI_H
#define SPCGANESHAENGINE_GGRAPHICSAPI_H

#include "ganeshatypes.h"
#include "gmatrix.h"
#include "gganeshacontent.h"

namespace spcGaneshaEngine {

class GGraphicsAPIProtocol {
public:
    virtual void initAPI(void *metalLayer, const TUInt frameWidth, const TUInt frameHeight, GGaneshaContent& content) = 0;
    virtual void destroyAPI() = 0;
    virtual void drawFrame() = 0;
    virtual void frameResized(const TFloat width, const TFloat height) = 0;
    virtual void installIsometricView(const TFloat fieldOfView, const TFloat near, const TFloat far) = 0;
    virtual void installViewMatrix(const GMatrix& newViewMatrix) = 0;
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GGRAPHICSAPI_H
