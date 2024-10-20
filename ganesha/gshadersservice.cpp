#include <vulkan/vulkan.h>

#include "gshadersservice.h"
#include "gvulkantools.h"

namespace spcGaneshaEngine {

GShadersService::GShadersService() {
    
}

GShadersService::~GShadersService() {
    
}

void GShadersService::init() {
    
}

void GShadersService::destroy(GVULKANDevice& vulkanDevice) {
}

void GShadersService::destroyShader(std::shared_ptr<GShader> shader, VkDevice device) {
    vkDestroyShaderModule(device, shader->getShaderModule(), nullptr);
}

bool GShadersService::isDeployed(std::shared_ptr<GShader> shader) {
    if (shader->getShaderModule() == VK_NULL_HANDLE) {
        return false;
    }
    
    return true;
}

void GShadersService::deployShader(std::shared_ptr<GShader> shader, GCommandServiceProtocol *commandService, GVULKANDevice& vulkanDevice) {
    GVULKANTools tools;
    const std::vector<uint8_t> code = tools.readFile(shader->getShaderFileName());
    VkShaderModuleCreateInfo createInfo = { };
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const TUInt*>(code.data());
    
    VkShaderModule newModule;
    if (vkCreateShaderModule(vulkanDevice.getLogicalDevice(), &createInfo, nullptr, &newModule) != VK_SUCCESS) {
        GLOG_ERROR("No chance to create shader module\n");
    }
    shader->setShaderModule(newModule);
}

std::shared_ptr<GShader> GShadersService::createFragmentShader(const std::string& shaderFile) {
    std::shared_ptr<GShader> newShader = std::make_shared<GShader>(shaderFile);
    return newShader;
}

std::shared_ptr<GShader> GShadersService::createVertexShader(const std::string& shaderFile) {
    std::shared_ptr<GShader> newShader = std::make_shared<GShader>(shaderFile);
    return newShader;
}

std::vector<VkPipelineShaderStageCreateInfo> GShadersService::getShadersPipelineInfo(std::vector<std::shared_ptr<GShader>>& shadersArray, const VkShaderStageFlagBits stage, GCommandServiceProtocol *commandService, GVULKANDevice& vulkanDevice) {
    std::vector<VkPipelineShaderStageCreateInfo> infoArray;
    
    for (std::shared_ptr<GShader> shader:shadersArray) {
        if (!isDeployed(shader)) {
            deployShader(shader, commandService, vulkanDevice);
        }
        infoArray.push_back(shaderPipelineInfo(shader, stage));
    }

    return infoArray;
}

VkPipelineShaderStageCreateInfo GShadersService::shaderPipelineInfo(std::shared_ptr<GShader> shader, const VkShaderStageFlagBits stage) {
    VkPipelineShaderStageCreateInfo shaderStageInfo = { };
    
    shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStageInfo.stage = stage;
    shaderStageInfo.module = shader->getShaderModule();
    shaderStageInfo.pName = "main";
    
    return shaderStageInfo;
}

}   //  spcGaneshaEngine
