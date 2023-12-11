//
//  gvulkantools.cpp
//  game
//
//  Created by cipher on 20.05.2022.
//

#include "vulkantools.h"

namespace spcGaneshaEngine {

TInstanceExtensionsArray collectInstanceExtensions() {
    //  define count of extensions
    uint32_t count = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
    
    //  collect all the extensions
    TInstanceExtensionsArray collectedList(count);
    vkEnumerateInstanceExtensionProperties(nullptr, &count, collectedList.data());
    
    return collectedList;
}

TCharPointersArray collectInstanceExtensionsNames(TInstanceExtensionsArray extensionsList) {
    TCharPointersArray namesList = TCharPointersArray();
    for (const auto& extension : extensionsList) {
        char *newString = new char[VK_MAX_EXTENSION_NAME_SIZE];
        memcpy(newString, extension.extensionName, VK_MAX_EXTENSION_NAME_SIZE);
        namesList.insert(namesList.end(), newString);
    }
    
    return namesList;
}

TCharPointersArray collectValidationLayers(const TCharPointersArray& layersNamesList) {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    
    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
    
    TCharPointersArray supportedValidationLayersList;
    for (const auto layerName : layersNamesList) {
        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                supportedValidationLayersList.push_back(layerName);
            }
        }
    }
    
    return supportedValidationLayersList;
}

}   //  namespace spcGaneshaEngine
