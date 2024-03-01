#ifndef SPCGANESHAENGINE_GSHADERSSERVICE_H
#define SPCGANESHAENGINE_GSHADERSSERVICE_H

#include <vulkan/vulkan.h>
#include <vector>
#include <string>

#include "gvulkandevice.h"

namespace spcGaneshaEngine {

typedef std::vector<VkPipelineShaderStageCreateInfo> TShadersArray;

class GShadersServiceProtocol {
public:
    virtual void init() = 0;
    virtual void destroy() = 0;
    virtual TShadersArray getAllShadersArray() = 0;
    virtual TShadersArray& getVertexShadersArray() = 0;
    virtual TShadersArray& getFrgamentShadersArray() = 0;
    virtual void addFragmentShader(const std::string& shaderFile) = 0;
    virtual void addVertexShader(const std::string& shaderFile) = 0;
};

class GShadersService: public GShadersServiceProtocol {
public:
    GShadersService(GVULKANDevice& vulkanDevice);
    virtual ~GShadersService();
    
    void init() override;
    void destroy() override;

    TShadersArray getAllShadersArray() override;
    TShadersArray& getVertexShadersArray() override;
    TShadersArray& getFrgamentShadersArray() override;
    void addFragmentShader(const std::string& shaderFile) override;
    void addVertexShader(const std::string& shaderFile) override;

private:
    GVULKANDevice& vulkanDevice;
    VkPipelineShaderStageCreateInfo createShader(const std::string& shaderFile,  const VkShaderStageFlagBits stage, VkDevice device);
    TShadersArray fragmentShadersArray;
    TShadersArray vertexShadersArray;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GSHADERSSERVICE_H
