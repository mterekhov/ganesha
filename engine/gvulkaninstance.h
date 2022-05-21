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

/// Instance of VULKAN render machine
class GVULKANInstance {
public:
    GVULKANInstance(const bool useValidationLayers = true);
    ~GVULKANInstance();
    
    /// returns reference to created VULKAN instance object
    VkInstance& getVulkanInstance();
    
private:
    VkInstance vulkanInstance;
    VkApplicationInfo applicationInfo;
    VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerInfo;
    VkDebugUtilsMessengerEXT debugMessenger;
    
    TCharPointersArray availableValidationLayersList;
    TInstanceExtensionsArray extensionsList;
    TCharPointersArray extensionsNamesList;
    
    void createNewInstance(const bool useValidationLayers);
    void destroyInstance();
    VkInstanceCreateInfo createInstanceInfo(const bool useValidationLayers);
    VkApplicationInfo createApplicationInfo();
    
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
