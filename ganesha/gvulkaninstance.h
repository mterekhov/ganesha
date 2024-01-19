#ifndef SPCGANESHAENGINE_GVULKANINSATANCE_H
#define SPCGANESHAENGINE_GVULKANINSATANCE_H

#include <vulkan/vulkan.h>

#include "ganeshatypes.h"
#include "glog.h"

namespace spcGaneshaEngine {

class GVULKANInstance {
public:
    GVULKANInstance();
    ~GVULKANInstance();
    
    void createInstance(const std::string& applicationName, const TCharPointersArray& khronosValidationLayers, const TStringsArray& shouldNotUseExtensionsArray);
    void destroyInstance();

    VkInstance getVulkanInstance();
    
private:
    VkInstance vulkanInstance;
    VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerInfo;
    VkDebugUtilsMessengerEXT debugMessenger;

    VkInstanceCreateInfo createInstanceInfo(const VkApplicationInfo& applicationInfo,
                                            const TCharPointersArray& availableValidationLayersList,
                                            const TCharPointersArray& extensionsNamesArray);
    VkApplicationInfo createApplicationInfo(const std::string& title);
    
    TCharPointersArray collectValidationLayers(const TCharPointersArray& layersNamesArray);
    TCharPointersArray collectInstanceExtensionsNames(const TStringsArray& shouldNotUseExtensionsArray);
    TBool shouldUseInstanceExtension(const char *instanceExtensionName, const TStringsArray& shouldNotUseExtensionsArray);
    
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
