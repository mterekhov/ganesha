#ifndef SPCGANESHAENGINE_GSHADER_H
#define SPCGANESHAENGINE_GSHADER_H

#include <vulkan/vulkan.h>

#include "ganeshatypes.h"
#include "gvulkanbuffer.h"
#include "gvulkanimage.h"
#include "gcommandservice.h"

namespace spcGaneshaEngine {

class GShader {
public:
    GShader(const std::string& shaderFileName);
    ~GShader();
    
    VkShaderModule getShaderModule();
    void setShaderModule(VkShaderModule newModule);
    std::string getShaderFileName();

private:
    const std::string shaderFileName;
    VkShaderModule shaderModule;
};

};  //  spcGaneshaEngine

#endif  //  SPCGANESHAENGINE_GSHADER_H
