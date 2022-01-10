//
//  ganeshatypes.h
//  ganesha
//
//  Created by Mihail Terekhov on 23.07.2021.
//

#ifndef SPCGANESHAENGINE_GANESHATYPES_H
#define SPCGANESHAENGINE_GANESHATYPES_H

#include <vector>
#include <vulkan/vulkan.h>

namespace spcGaneshaEngine {

#define FLOAT_PRECISION 0.000001f
#define DEG_TO_RAD(deg)    ((deg) * M_PI / 180.0f)
#define RAD_TO_DEG(rad) ((rad) * 180.0f / M_PI)

typedef std::vector<VkExtensionProperties> TInstanceExtensionsArray;
typedef std::vector<const char *> TCharPointersArray;
typedef std::vector<std::string> TStringsArray;
typedef float TFloat;

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GANESHATYPES_H
