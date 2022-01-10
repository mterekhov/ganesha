//
//  gvulkaninstance.hpp
//  ganesha
//
//  Created by Mihail Terekhov on 23.07.2021.
//

#ifndef SPCGANESHAENGINE_GVULKANINSATANCE_H
#define SPCGANESHAENGINE_GVULKANINSATANCE_H

#include "ganeshatypes.h"

namespace spcGaneshaEngine {

class GVULKANInstance {
public:
    GVULKANInstance();
    ~GVULKANInstance();

    void init(const bool useValidationLayers);
    VkInstance& getVulkanInstance();
    
private:
    VkInstance vulkanInstance;
    VkApplicationInfo applicationInfo;
    VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerInfo;
    VkDebugUtilsMessengerEXT debugMessenger;
    
    TCharPointersArray availableValidationLayersList;
    TInstanceExtensionsArray extensionsList;
    TCharPointersArray extensionsNamesList;
    
    TInstanceExtensionsArray collectInstanceExtensions();
    TCharPointersArray collectInstanceExtensionsNames(TInstanceExtensionsArray extensionsList);
    VkInstanceCreateInfo createInstanceInfo(const bool useValidationLayers);
    VkApplicationInfo createApplicationInfo();
    
    TCharPointersArray collectValidationLayers(const TCharPointersArray& layersNamesList);
    VkDebugUtilsMessengerCreateInfoEXT createDebugUtilsMessengerInfo();
    VkResult createDebugUtilsMessenger(VkInstance instance,
                                       const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                       const VkAllocationCallbacks* pAllocator,
                                       VkDebugUtilsMessengerEXT* pDebugMessenger);
    void destroyDebugUtilsMessenger(VkInstance instance,
                                    VkDebugUtilsMessengerEXT debugMessenger,
                                    const VkAllocationCallbacks* pAllocator);

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                        VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                        void* pUserData);
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GVULKANINSATANCE_H
