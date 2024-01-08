#ifndef SPCGANESHAENGINE_GSPRITENODE_H
#define SPCGANESHAENGINE_GSPRITENODE_H

#include <string>

#include "gnode.h"

namespace spcGaneshaEngine {

class GSpriteNode: public GNode {
public:
    GSpriteNode(const std::string& textureFilePath);
    virtual ~GSpriteNode();

    virtual void render();
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GSPRITENODE_H
