#include "gshadersservice.h"
#include "gvulkantools.h"

namespace spcGaneshaEngine {

GShadersService::GShadersService(GVULKANDevice& vulkanDevice) : vulkanDevice(vulkanDevice) {
    
}

GShadersService::~GShadersService() {
    
}

void GShadersService::init() {
    
}

void GShadersService::destroy() {
    VkDevice device = vulkanDevice.getLogicalDevice();
    for (auto shader:fragmentShadersArray) {
        vkDestroyShaderModule(device, shader.module, nullptr);
    }
    fragmentShadersArray.clear();
    
    for (auto shader:vertexShadersArray) {
        vkDestroyShaderModule(device, shader.module, nullptr);
    }
    vertexShadersArray.clear();
}

TShadersArray GShadersService::getAllShadersArray() {
    TShadersArray result = vertexShadersArray;

    result.insert(result.end(), fragmentShadersArray.begin(), fragmentShadersArray.end());

    return result;
}

void GShadersService::addFragmentShader(const std::string& shaderFile) {
    fragmentShadersArray.push_back(createShader(shaderFile, VK_SHADER_STAGE_FRAGMENT_BIT, vulkanDevice.getLogicalDevice()));
}

TShadersArray& GShadersService::getFrgamentShadersArray() {
    return fragmentShadersArray;
}

void GShadersService::addVertexShader(const std::string& shaderFile) {
    vertexShadersArray.push_back(createShader(shaderFile, VK_SHADER_STAGE_VERTEX_BIT, vulkanDevice.getLogicalDevice()));
}

TShadersArray& GShadersService::getVertexShadersArray() {
    return vertexShadersArray;
}

VkPipelineShaderStageCreateInfo GShadersService::createShader(const std::string& shaderFile, const VkShaderStageFlagBits stage, VkDevice device) {
    GVULKANTools tools;
    const std::vector<uint8_t> code = tools.readFile(shaderFile);
    VkShaderModuleCreateInfo createInfo = { };
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const TUInt*>(code.data());
    
    VkShaderModule shaderModule;
    if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        GLOG_ERROR("No chance to create shader module\n");
    }
    
    VkPipelineShaderStageCreateInfo shaderStageInfo = { };
    shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStageInfo.stage = stage;
    shaderStageInfo.module = shaderModule;
    shaderStageInfo.pName = "main";
    
    return shaderStageInfo;
}

}   //  spcGaneshaEngine
