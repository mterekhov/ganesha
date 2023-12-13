//
//  gvulkaninstance.hpp
//  ganesha
//
//  Created by Mihail Terekhov on 23.07.2021.
//

#ifndef SPCGANESHAENGINE_GVULKANINSATANCE_H
#define SPCGANESHAENGINE_GVULKANINSATANCE_H

#include "ganeshatypes.h"
#include <vulkan/vulkan.h>

namespace spcGaneshaEngine {

/// Instance of VULKAN render machine
class GVULKANInstance {
public:
    void createInstance(const bool useValidationLayers);
    void destroyInstance();

    /// returns reference to created VULKAN instance object
    VkInstance& getVulkanInstance();
    
private:
    VkInstance vulkanInstance;
    VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerInfo;
    VkDebugUtilsMessengerEXT debugMessenger;

    void createNewInstance(const bool useValidationLayers);
    VkInstanceCreateInfo createInstanceInfo(const VkApplicationInfo& applicationInfo,
                                            const TCharPointersArray availableValidationLayersList,
                                            const TCharPointersArray& extensionsNamesArray);
    VkApplicationInfo createApplicationInfo();
    
    TCharPointersArray collectValidationLayers(const TCharPointersArray& layersNamesArray);
    TCharPointersArray collectInstanceExtensionsNames();

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
