#ifndef SPCGANESHAENGINE_GSHADERSSERVICE_H
#define SPCGANESHAENGINE_GSHADERSSERVICE_H

#include <vulkan/vulkan.h>
#include <vector>
#include <string>

#include "gvulkandevice.h"
#include "gshader.h"

namespace spcGaneshaEngine {


class GShadersServiceProtocol {
public:
    virtual void init() = 0;
    virtual void destroy(GVULKANDevice& vulkanDevice) = 0;
    virtual void deployShader(std::shared_ptr<GShader> shader, 
                              GVULKANDevice& vulkanDevice) = 0;
    virtual std::shared_ptr<GShader> createFragmentShader(const std::string& shaderFile) = 0;
    virtual std::shared_ptr<GShader> createVertexShader(const std::string& shaderFile) = 0;
    virtual std::vector<VkPipelineShaderStageCreateInfo> getShadersPipelineInfo(std::vector<std::shared_ptr<GShader>>& shadersArray,
                                                                                const VkShaderStageFlagBits stage,
                                                                                GVULKANDevice& vulkanDevice) = 0;
    virtual void destroyShader(std::shared_ptr<GShader> shader, VkDevice device) = 0;
    virtual bool isDeployed(std::shared_ptr<GShader> shader) = 0;
};

class GShadersService: public GShadersServiceProtocol {
public:
    GShadersService();
    virtual ~GShadersService();
    
    void init() override;
    void destroy(GVULKANDevice& vulkanDevice) override;
    void deployShader(std::shared_ptr<GShader> shader, 
                      GVULKANDevice& vulkanDevice) override;
    std::vector<VkPipelineShaderStageCreateInfo> getShadersPipelineInfo(std::vector<std::shared_ptr<GShader>>& shadersArray, 
                                                                        const VkShaderStageFlagBits stage,
                                                                        GVULKANDevice& vulkanDevice) override;
    std::shared_ptr<GShader> createFragmentShader(const std::string& shaderFile) override;
    std::shared_ptr<GShader> createVertexShader(const std::string& shaderFile) override;
    void destroyShader(std::shared_ptr<GShader> shader, VkDevice device) override;
    bool isDeployed(std::shared_ptr<GShader> shader) override;

private:
    VkPipelineShaderStageCreateInfo shaderPipelineInfo(std::shared_ptr<GShader> shader, const VkShaderStageFlagBits stage);
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GSHADERSSERVICE_H
