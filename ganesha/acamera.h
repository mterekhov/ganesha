#ifndef SPCGANESHAENGINE_GCAMERA_H
#define SPCGANESHAENGINE_GCAMERA_H

#include "ganeshatypes.h"

namespace spcGaneshaEngine {

class GCamera {
public:
    TFloat cSpeed;
    TFloat sens;
    AVector position;
    AVector view;
    AVector up;
    AVector strafe;
    
    GCamera(TFloat = 5, TFloat = 5, TFloat = 5,
            TFloat = 4, TFloat = 4, TFloat = 4,
            TFloat = 0, TFloat = 1, TFloat = 0);

    TFloat Magnitude(AVector normal);
    AVector Cross(AVector vector_1, AVector vector_2);
    AVector Normalize(AVector vector);
    void moveCamera(TFloat dir);
    void rotateCamera(TFloat dir, TFloat x, TFloat y, TFloat z);
    void strafeCamera(TFloat dir);
    void upDownCamera(TFloat dir);
    void setMouseView();
    void posCamera(TFloat posX, TFloat posY, TFloat posZ,
                   TFloat viewX, TFloat viewY, TFloat viewZ,
                   TFloat upX, TFloat upY, TFloat upZ);
};

};

#endif
