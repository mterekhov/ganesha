#ifndef SPCGANESHAENGINE_GDESCRIPTORSETSERVICE_H
#define SPCGANESHAENGINE_GDESCRIPTORSETSERVICE_H

#include <vulkan/vulkan.h>

#include "gvulkanbuffer.h"
#include "gvulkanimage.h"

namespace spcGaneshaEngine {

class GDescriptorsetServiceProtocol {
public:
    virtual void init() = 0;
    virtual void destroy() = 0;
    
    virtual VkDescriptorSetLayout getDescriptorsetLayout() = 0;
    virtual VkDescriptorSet getDescriptorset() = 0;
    virtual void attachImageToDescriptorset(GVULKANImage& image, TUInt bindingIndex) = 0;
    virtual void attachBufferToDescriptorset(GVULKANBuffer& buffer, TUInt bindingIndex) = 0;
};

class GDescriptorsetService: public GDescriptorsetServiceProtocol {
public:
    GDescriptorsetService(GVULKANDevice& vulkanDevice);
    virtual ~GDescriptorsetService();
    
    virtual void init();
    virtual void destroy();
    virtual VkDescriptorSetLayout getDescriptorsetLayout();
    virtual VkDescriptorSet getDescriptorset();
    
    virtual void attachImageToDescriptorset(GVULKANImage& image, TUInt bindingIndex);
    virtual void attachBufferToDescriptorset(GVULKANBuffer& buffer, TUInt bindingIndex);

private:
    GVULKANDevice& vulkanDevice;
    VkDescriptorPool descriptorsetsPool;
    VkDescriptorSetLayout descriptorsetsLayout;
    VkDescriptorSet descriptorset;

    VkDescriptorPool createDescriptorsetsPool(VkDevice device);
    VkDescriptorSetLayout createDescriptorsetsLayout(VkDevice device);
    VkDescriptorSet createDescriptorset(VkDescriptorPool pool, VkDescriptorSetLayout layout, VkDevice device);
};

};

#endif
