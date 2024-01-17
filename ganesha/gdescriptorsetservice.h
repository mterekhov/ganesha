#ifndef SPCGANESHAENGINE_GDESCRIPTORSETSERVICE_H
#define SPCGANESHAENGINE_GDESCRIPTORSETSERVICE_H

#include <vulkan/vulkan.h>

#include "gvulkanbuffer.h"
#include "gvulkanimage.h"

namespace spcGaneshaEngine {

class GDescriptorsetServiceProtocol {
public:
    virtual void initService(VkDevice device) = 0;
    virtual void destroy(VkDevice device) = 0;
    virtual VkDescriptorSetLayout getDescriptorsetLayout() = 0;
    virtual VkDescriptorSet getDescriptorset() = 0;
    virtual void attachImageToDescriptorset(GVULKANImage& image, TUInt bindingIndex, VkDevice device) = 0;
    virtual void attachBufferToDescriptorset(GVULKANBuffer& buffer, TUInt bindingIndex, VkDevice device) = 0;
};

class GDescriptorsetService: public GDescriptorsetServiceProtocol {
public:
    GDescriptorsetService();
    virtual ~GDescriptorsetService();
    
    void initService(VkDevice device);
    void destroy(VkDevice device);
    VkDescriptorSetLayout getDescriptorsetLayout();
    VkDescriptorSet getDescriptorset();
    
    void attachImageToDescriptorset(GVULKANImage& image, TUInt bindingIndex, VkDevice device);
    void attachBufferToDescriptorset(GVULKANBuffer& buffer, TUInt bindingIndex, VkDevice device);

private:
    VkDescriptorPool descriptorsetsPool;
    VkDescriptorSetLayout descriptorsetsLayout;
    VkDescriptorSet descriptorset;

    VkDescriptorPool createDescriptorsetsPool(VkDevice device);
    VkDescriptorSetLayout createDescriptorsetsLayout(VkDevice device);
    VkDescriptorSet createDescriptorset(VkDescriptorPool pool, VkDescriptorSetLayout layout, VkDevice device);
};

};

#endif
