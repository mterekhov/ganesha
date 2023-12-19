#ifndef SPCGANESHAENGINE_GVULKANINSATANCE_H
#define SPCGANESHAENGINE_GVULKANINSATANCE_H

#include <vulkan/vulkan.h>

#include "ganeshatypes.h"
#include "glog.h"

namespace spcGaneshaEngine {

/// Instance of VULKAN render machine
class GVULKANInstance {
public:
    GVULKANInstance(GLog& log);
    ~GVULKANInstance();
    
    void createInstance(const std::string& applicationName, const TCharPointersArray& khronosValidationLayers);
    void destroyInstance();

    /// returns reference to created VULKAN instance object
    VkInstance& getVulkanInstance();
    void addInstanceExtensionsToAvoid(const TStringsArray& extensions);
    
private:
    GLog& log;
    VkInstance vulkanInstance;
    VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerInfo;
    VkDebugUtilsMessengerEXT debugMessenger;
    TStringsArray removeExtensions;

    VkInstanceCreateInfo createInstanceInfo(const VkApplicationInfo& applicationInfo,
                                            const TCharPointersArray& availableValidationLayersList,
                                            const TCharPointersArray& extensionsNamesArray);
    VkApplicationInfo createApplicationInfo(const std::string& title);
    
    TCharPointersArray collectValidationLayers(const TCharPointersArray& layersNamesArray);
    TCharPointersArray collectInstanceExtensionsNames();
    bool shouldUseInstanceExtension(const char *instanceExtensionName);
    
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
