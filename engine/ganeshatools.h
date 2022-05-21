//
//  gvulkantools.hpp
//  game
//
//  Created by cipher on 20.05.2022.
//

#ifndef SPCGANESHAENGINE_GANESHATOOLS_H
#define SPCGANESHAENGINE_GANESHATOOLS_H

#include "ganeshatypes.h"

namespace spcGaneshaEngine {

/// Collects extensions for VULKAN instance, e.g. VK_EXT_metal_surface, VK_EXT_debug_utils, VK_KHR_external_semaphore_capabilities, etc
TInstanceExtensionsArray collectInstanceExtensions();

/// Extracts names only from array of VkExtensionProperties and returns array of char*
/// @param extensionsList array of VkExtensionProperties
TCharPointersArray collectInstanceExtensionsNames(TInstanceExtensionsArray extensionsList);

/// Collects array of VkLayerProperties and matches with layersNamesList then returns the one which are equal
/// @param layersNamesList array of char* which are names of layers
TCharPointersArray collectValidationLayers(const TCharPointersArray& layersNamesList);

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GANESHATOOLS_H
