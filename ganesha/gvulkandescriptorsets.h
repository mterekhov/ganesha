#ifndef SPCGANESHAENGINE_GVULKANDESCRIPTORSETS_H
#define SPCGANESHAENGINE_GVULKANDESCRIPTORSETS_H

#include <vulkan/vulkan.h>

#include "gvulkandevice.h"
#include "glob.h"
#include "gvulkanbuffer.h"

namespace spcGaneshaEngine {

class GVULKANDescriptorsets {
public:
    GVULKANDescriptorsets(GLog& log);
    ~GVULKANDescriptorsets();
    
    void createDescriptorsets(GVULKANDevice& vulkanDevice, std::vector<GVULKANBuffer>& buffersArray);
    void destroyDescriptorsets(GVULKANDevice& vulkanDevice);
    
    std::vector<VkDescriptorSet>& getDescriptorsetArray();
    VkDescriptorSetLayout getDescriptorsetLayout();

private:
    VkDescriptorSetLayout createLayout(VkDevice device);
    VkDescriptorPool createDescriptorPool(const TUInt descriptorsPoolSize, VkDevice device);
    std::vector<VkDescriptorSet> createNewDescriptorsets(std::vector<GVULKANBuffer>& buffersArray, VkDescriptorSetLayout descriptorsetLayout, VkDevice device);

    GLog& log;
    std::vector<VkDescriptorSet> descriptorsetArray;
    VkDescriptorSetLayout layout;
    VkDescriptorPool pool;
    TUInt poolSize;
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GVULKANAPI_H
