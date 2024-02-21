#ifndef SPCGANESHAENGINE_GMATRIX_H
#define SPCGANESHAENGINE_GMATRIX_H

#include "ganeshatypes.h"
#include "gpoint.h"

namespace spcGaneshaEngine {
    
struct GMatrix {
public:
    TFloat m[4][4];
    
    const std::string print() const;
    
    void transponate();
    
    static GMatrix zeroMatrix();
    static GMatrix identityMatrix();
    static GMatrix rotationX(const TFloat angle);
    static GMatrix rotationY(const TFloat angle);
    static GMatrix rotationZ(const TFloat angle);
    static GMatrix frustum(const TFloat left,
                           const TFloat right,
                           const TFloat bottom,
                           const TFloat top,
                           const TFloat near,
                           const TFloat far);
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GMATRIX_H
