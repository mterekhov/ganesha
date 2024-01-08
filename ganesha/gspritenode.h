#ifndef SPCGANESHAENGINE_GSPRITENODE_H
#define SPCGANESHAENGINE_GSPRITENODE_H

#include "gnode.h"

namespace spcGaneshaEngine {

class GSpriteNode: public GNode {
public:
    GSpriteNode();
    virtual ~GSpriteNode();

    virtual void render();
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GSPRITENODE_H
