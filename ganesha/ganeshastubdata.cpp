//
//  ganeshastubdata.cpp
//  game
//
//  Created by cipher on 21.05.2022.
//

#include <vector>

#include "ganeshastubdata.h"

namespace spcGaneshaEngine {

const std::vector<Vertex> vertices = {
    {GPoint2D(-0.5f, -0.5f), GColor::blueColor()},
    {GPoint2D(0.5f, -0.5f), GColor::redColor()},
    {GPoint2D(0.5f, 0.5f), GColor::greenColor()},
    {GPoint2D(-0.5f, 0.5f), GColor::redColor()}
};

const std::vector<uint16_t> indices = {
    0, 1, 2, 2, 3, 0
};

}   //  namespace spcGaneshaEngine
