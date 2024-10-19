//
//  ganeshatypes.h
//  ganesha
//
//  Created by Mihail Terekhov on 23.07.2021.
//

#ifndef SPCGANESHAENGINE_GANESHATYPES_H
#define SPCGANESHAENGINE_GANESHATYPES_H

#include <vector>
#include <string>
#include <vulkan/vulkan.h>

#include "gshader.h"

namespace spcGaneshaEngine {

/// Precision for comparison of float values
#define FLOAT_PRECISION std::numeric_limits<TFloat>::min()

/// converts degrees into radians
#define DEG_TO_RAD(deg) ((deg) * M_PI / 180.0f)

/// converts radians into degrees
#define RAD_TO_DEG(rad) ((rad) * 180.0f / M_PI)

typedef float TFloat;
typedef bool TBool;
typedef uint32_t TUInt;
typedef TUInt TIndex;
typedef std::vector<TIndex> TIndexArray;
typedef std::vector<const char *> TCharPointersArray;
typedef std::vector<std::string> TStringsArray;

typedef std::vector<std::shared_ptr<GShader>> TShadersArray;
typedef std::vector<VkPipelineShaderStageCreateInfo> TShadersPipelineInfoArray;

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GANESHATYPES_H
