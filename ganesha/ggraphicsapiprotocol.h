#ifndef SPCGANESHAENGINE_GGRAPHICSAPI_H
#define SPCGANESHAENGINE_GGRAPHICSAPI_H

#include "ganeshatypes.h"
#include "gmatrix.h"
#include "gscene.h"

namespace spcGaneshaEngine {

class GGraphicsAPIProtocol {
public:
    virtual void initAPI(void *metalLayer, const GScene& content) = 0;
    virtual void destroyAPI() = 0;
    virtual void render() = 0;
    virtual void updateSwapChain(const GViewport& viewport) = 0;
    virtual void installIsometricProjection(const GViewport& viewport) = 0;
    virtual void installViewMatrix(const GMatrix& newViewMatrix) = 0;
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GGRAPHICSAPI_H
