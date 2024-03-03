#ifndef SPCGANESHAENGINE_GMATRIX_H
#define SPCGANESHAENGINE_GMATRIX_H

#include "ganeshatypes.h"
#include "gpoint.h"

namespace spcGaneshaEngine {
    
struct GMatrix {
public:
    TFloat m[4][4] = { 0 };
    
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
    
    inline const GMatrix& operator*=(const GMatrix& src) {
        float temp[4][4] = { 0 };
        
        for (int i = 0; i < 4; ++i)
        {
            temp[0][i] = m[0][i] * src.m[0][0] + m[1][i] * src.m[0][1] + m[2][i] * src.m[0][2] + m[3][i] * src.m[0][3];
            temp[1][i] = m[0][i] * src.m[1][0] + m[1][i] * src.m[1][1] + m[2][i] * src.m[1][2] + m[3][i] * src.m[1][3];
            temp[2][i] = m[0][i] * src.m[2][0] + m[1][i] * src.m[2][1] + m[2][i] * src.m[2][2] + m[3][i] * src.m[2][3];
            temp[3][i] = m[0][i] * src.m[3][0] + m[1][i] * src.m[3][1] + m[2][i] * src.m[3][2] + m[3][i] * src.m[3][3];
        }
        
        memcpy(m, temp, 16 * sizeof(float));
        
        return *this;
    }
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GMATRIX_H
