#ifndef SPCGANESHAENGINE_GSHADERSSERVICE_H
#define SPCGANESHAENGINE_GSHADERSSERVICE_H

#include <vulkan/vulkan.h>
#include <vector>
#include <string>

#include "gvulkandevice.h"

namespace spcGaneshaEngine {

class GShadersServiceProtocol {
public:
    virtual void init() = 0;
    virtual void destroy() = 0;
    virtual std::vector<VkPipelineShaderStageCreateInfo> getAllShadersArray() = 0;
    virtual std::vector<VkPipelineShaderStageCreateInfo>& getVertexShadersArray() = 0;
    virtual std::vector<VkPipelineShaderStageCreateInfo>& getFrgamentShadersArray() = 0;
    virtual void addFragmentShader(const std::string& shaderFile) = 0;
    virtual void addVertexShader(const std::string& shaderFile) = 0;
};

class GShadersService: public GShadersServiceProtocol {
public:
    GShadersService(GVULKANDevice& vulkanDevice);
    virtual ~GShadersService();
    
    virtual void init();
    virtual void destroy();

    virtual std::vector<VkPipelineShaderStageCreateInfo> getAllShadersArray();
    virtual std::vector<VkPipelineShaderStageCreateInfo>& getVertexShadersArray();
    virtual std::vector<VkPipelineShaderStageCreateInfo>& getFrgamentShadersArray();
    virtual void addFragmentShader(const std::string& shaderFile);
    virtual void addVertexShader(const std::string& shaderFile);

private:
    GVULKANDevice& vulkanDevice;
    VkPipelineShaderStageCreateInfo createShader(const std::string& shaderFile,  const VkShaderStageFlagBits stage, VkDevice device);
    std::vector<VkPipelineShaderStageCreateInfo> fragmentShadersArray;
    std::vector<VkPipelineShaderStageCreateInfo> vertexShadersArray;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GSHADERSSERVICE_H
