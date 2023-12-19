//
//  gvulkaninstance.hpp
//  ganesha
//
//  Created by Mihail Terekhov on 23.07.2021.
//

#ifndef SPCGANESHAENGINE_GVULKANPIPELINE_H
#define SPCGANESHAENGINE_GVULKANPIPELINE_H

#include <vulkan/vulkan.h>
#include <array>

#include "glog.h"
#include "gvulkandevice.h"
#include "gvulkanswapchain.h"
#include "ganeshastubdata.h"

namespace spcGaneshaEngine {

class GVULKANPipeline {
public:
    GVULKANPipeline(GLog& log);
    ~GVULKANPipeline();
    
    void createPipeline(GVULKANDevice& device, GVULKANSwapChain& swapChain, VkDescriptorSetLayout& descriptorsetLayout);
    void destroyPipeline(GVULKANDevice& device);
    VkPipeline& getGraphicsPipeline();
    VkPipelineLayout& getPipelineLayout();

private:
    GLog& log;
    VkVertexInputBindingDescription bindingDescription = Vertex::getBindingDescription();
    std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = Vertex::getAttributeDescriptions();
    VkPipeline graphicsPipeline;
    VkPipelineLayout pipelineLayout;

    VkPipelineShaderStageCreateInfo createShader(const std::string shaderFile,  const VkShaderStageFlagBits stage, VkDevice& device);
    VkPipelineVertexInputStateCreateInfo createVertexInput();
    VkPipelineViewportStateCreateInfo createViewport(const VkExtent2D& extent);
    VkPipelineRasterizationStateCreateInfo createRasterizer();
    VkPipelineColorBlendStateCreateInfo createColorBlending();
    VkPipelineLayout createPipelineLayout(const VkDevice& device, VkDescriptorSetLayout& descriptorsetLayout);
};

}   //  namespace spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GVULKANPIPELINE_H
