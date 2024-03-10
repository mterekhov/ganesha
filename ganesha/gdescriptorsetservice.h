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
    virtual void updateDescriptorset(GVULKANImage& image, TUInt bindingIndex) = 0;
    virtual void updateDescriptorset(GVULKANBuffer& buffer, TUInt bindingIndex) = 0;
    virtual std::vector<VkVertexInputBindingDescription> getBindingDescription() = 0;
    virtual std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() = 0;
};

class GDescriptorsetService: public GDescriptorsetServiceProtocol {
public:
    GDescriptorsetService(GVULKANDevice& vulkanDevice);
    virtual ~GDescriptorsetService();
    
    void init() override;
    void destroy() override;
    VkDescriptorSetLayout getDescriptorsetLayout() override;
    VkDescriptorSet getDescriptorset() override;
    
    void updateDescriptorset(GVULKANImage& image, TUInt bindingIndex) override;
    void updateDescriptorset(GVULKANBuffer& buffer, TUInt bindingIndex) override;

    std::vector<VkVertexInputBindingDescription> getBindingDescription() override;
    std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() override;
    
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
