#ifndef SPCGANESHAENGINE_GGRAPHICSAPI_H
#define SPCGANESHAENGINE_GGRAPHICSAPI_H

#include "ganeshatypes.h"
#include "gmatrix.h"

namespace spcGaneshaEngine {

class GGraphicsAPI {
public:
    virtual void initAPI(void *metalLayer, const uint32_t frameWidth, const uint32_t frameHeight) = 0;
    virtual void destroyAPI() = 0;
    virtual void drawFrame() = 0;
    virtual void frameResized(const TFloat width, const TFloat height) = 0;
    virtual void installIsometricView(const TFloat fieldOfView, const TFloat near, const TFloat far) = 0;
    virtual void installViewMatrix(const GMatrix& newViewMatrix) = 0;
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GGRAPHICSAPI_H
